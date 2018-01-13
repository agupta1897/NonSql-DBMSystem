#include"scanner.h"
#include"lex.h"
#include"recognizer.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<strings.h>

lexeme* currentLex;
FILE *fp;

int check( types type) 
{ 
        return currentLex->type == type; 
    }
lexeme* advance() 
        { 
        lexeme *temp = currentLex;
        currentLex = lex(fp); 
        return temp;
        } 

  
void matchNoAdvance(types type)
        {
        if (!check(type))
        {
            fprintf(stdout,"SYNTAX ERROR ~~ Line No: %d Expected -", currentLex->lineNum );
            lexeme *temp = malloc(sizeof(lexeme));
            temp->type = type;
            Display(stdout, temp);
            fprintf(stdout,"GOT: ");
            Display(stdout, currentLex);
            exit(-1);
        }
    }

lexeme* match( types type) 
        { 
        matchNoAdvance(type); 
        return advance(); 
    }
    
int collectionPending()
{
    return check(COLLECTION);
}

int queryPending()
{
    return check(QUERY);
}

int countPending()
{
    return check(COUNT);
}

int sortPending()
{
    return check(SORT);
}

int integerPending()
{
    return check(INTEGER);
}

int oparenPending()
{
    return check(OPAREN);
}

int equalsPending()
{
    return check(EQUALS);
}
int greaterthanPending()
{
    return check(GREATERTHAN);
}
int lessthanPending()
{
    return check(LESSTHAN);
}
int greaterthanequalPending()
{
    return check(GREATERTHANEQUAL);
}
int lessthanequalPending()
{
    return check(LESSTHANEQUAL);
}
int commaPending()
{
    return check(COMMA);
}

int cparenPending()
{
    return check(CPAREN);
}

int insertPending()
{
    return check(INSERT);
}

int fieldPending()
{
    return check(FIELD);
}


lexeme *optionalConditons()
{
    lexeme* t = newLex(AND);

    t->left= match(FIELD);

    if(equalsPending())
    t->right = match(EQUALS);
    else
    if(greaterthanPending())
    t->right = match(GREATERTHAN);
    else
    if(greaterthanequalPending())
    t->right = match(GREATERTHANEQUAL);
    else
    if(lessthanPending())
    t->right = match(LESSTHAN);
    else
    if(lessthanequalPending())
    t->right = match(LESSTHANEQUAL);
    else
    t->right = match(NOT);

    t->right->left = match(INTEGER);
     if (commaPending())
     {
        match(COMMA);
         t->right->right = optionalConditons();
        }
    return t;
}

lexeme* condition()
{
    lexeme *t = newLex(COND);
    t->left= match(FIELD);

    if(equalsPending())
    t->right = match(EQUALS);
    else
    if(greaterthanPending())
    t->right = match(GREATERTHAN);
    else
    if(greaterthanequalPending())
    t->right = match(GREATERTHANEQUAL);
    else
    if(lessthanPending())
    t->right = match(LESSTHAN);
    else
    if(lessthanequalPending())
    t->right = match(LESSTHANEQUAL);
    else
    t->right = match(NOT);

    t->right->left = match(INTEGER);
     if (commaPending())
     {
         match(COMMA);
         t->right->right = optionalConditons();
        }
    match(CSBRACKET);
    return t;

}

lexeme* projection()
{
    lexeme* t = match(FIELD);
    if(commaPending())
    {
        match(COMMA);
        t->left = projection();  
    }
    return t;
}

lexeme* version()
{
    lexeme* t;
    match(OSBRACKET);
    if(integerPending())
    t = match(INTEGER);
    else
    {
        t = newLex(INTEGER);
        t->integerVal = 0;
    }
    match(CSBRACKET);
    return t;

}

lexeme* query()
{
    lexeme * t = match(QUERY);
    match(OPAREN);
    match(OSBRACKET);
    if(fieldPending())
    t->left = condition();
    else
    match(CSBRACKET);
    match(COMMA);
    match(OSBRACKET);
    t->right = newLex(NEXT);
    if(fieldPending())

    t->right->left = projection();
    match(CSBRACKET);

    if(commaPending())
    {
        match(COMMA);
        t->right->right = version();

    }
    match(CPAREN);

    return t;
}


lexeme *count()
{
    lexeme *t = match(COUNT);
    match(OPAREN);
    match(OSBRACKET);
    t->left = match(FIELD);
    match(CSBRACKET);
    if(commaPending())
    {
        match(COMMA);
        t->right = version();   
    }
    match(CPAREN);
    return t;
}

lexeme *sort()
{
    lexeme *t = match(SORT);
    match(OPAREN);
    match(OSBRACKET);
    t->left = match(FIELD);
    match(CSBRACKET);
    if(commaPending())
    {
        match(COMMA);
        t->right = version();   
    }
    match(CPAREN);
    return t;
}

lexeme *insertOptionalAttributes()
{
    lexeme *t = match(FIELD);
    match(COLON);
    t->left = match(INTEGER);
    if(cparenPending())
    {
        match(CPAREN);
        return t;
    }
    else
    {
        t->right = insertOptionalAttributes();
    }
    return t;
}

lexeme *insert()
{
    lexeme *t = match(INSERT);
    match(OPAREN);
    t->right = match(FIELD);
    match(COLON);
    t->right->left = match(INTEGER);

    if(cparenPending())
    {
        match(CPAREN);
    }
    else
    {
        t->right->right = insertOptionalAttributes();
    }
    return t;
}

lexeme *statement()
{
    lexeme* t = newLex(NEXT);
    match(COLLECTION);
    match(DOT);
    if(queryPending())
    t->left = query();
    else
    if(countPending())
    t->left = count();
    else
    if(sortPending())
    t->left = sort();
    else
    t->left = insert();
    return t;
}



int indent = 0;
int nextcount = 0;
lexeme* parent;
int flag1 = 0;

lexeme* program( lexeme *t)
{
    lexeme *x = parent;
    if (flag1 == 0)
    {
        parent = t;
        flag1 = 1;
    }

    if (!check(END_OF_INPUT))
    {
       t->right = statement();
        return program(t->right);
        }
    else
    return x;

    
}

void indentprinter(FILE *fp)
    {
        for (int i = 0 ; i< indent; i++)
        {
            fprintf(fp," ");
        }
    }
    void printTree(FILE *fp, lexeme* tree, int flag){

        if (tree == NULL)
        {   
        return;
        }

        if(flag == 1)
        {
            indentprinter(fp);
            fprintf(fp, "Left  : ");
            Display(fp, tree);
        }
        else
        {
            indentprinter(fp);
            fprintf(fp, "Right : ");
            Display(fp, tree);
        }
        
        indent++;
        indent++;
        printTree(fp, tree->left, 1);
        
        printTree(fp,tree->right, 2);
        indent--;
        indent--;
    }

lexeme* recognize(FILE *file)
{   
    lexeme *t = newLex(PROGRAM);
    fp = file;
    currentLex = lex(fp);
   // Display(stdout, currentLex);
    lexeme *x;
    x = program(t);
   // Display(stdout, x);
    
    return x;
}

