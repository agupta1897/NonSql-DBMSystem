#include"scanner.h"
#include"lex.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<strings.h>
int lineNum = 1;
FILE *fp;
lexeme* newLex (types t )
{
    //fprintf(stdout,"got: %d\n", t);
    lexeme *token = malloc(sizeof(lexeme));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    token->type = t;
   // Display(stdout, token);
   // fprintf(stdout,"-------\n");
    return token;
}

 void skipWhiteSpace(FILE *fp)
{
int ch;

/* read chars until a non-whitespace character is encountered */

while ((ch = fgetc(fp)) != EOF && (isspace(ch) || ch == '`'))
{
    if (ch == '\n')
    lineNum++;
    if (ch == '`')
    {
    readLine(fp);
    lineNum++;
    }
   // fprintf(stdout, "SKIPPING:%c\n", ch);
}

/* a non-space character got us out of the loop, so push it back */

if (ch != EOF) ungetc(ch,fp);

}

lexeme* newIntLex ( FILE* fp, int negative)
{
    lexeme *token = malloc(sizeof(lexeme));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    int value = 0;
    fscanf(fp,"%d",&value);
    if(negative == 1)
    token->integerVal = value* -1;
    else
    token->integerVal = value;
    token->type = INTEGER;
        token->lineNum = lineNum;
return token;

}

lexeme* newFieldLex ( FILE* fp)
{
    lexeme *token = malloc(sizeof(lexeme));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    char *keyword = readToken(fp);
    if( strcasecmp(keyword, "query") == 0) token->type = QUERY; else
    if( strcasecmp(keyword, "count") == 0) token->type = COUNT; else
    if( strcasecmp(keyword, "sort") == 0) token->type = SORT; else
    if( strcasecmp(keyword, "insert") == 0) token->type = INSERT; else
    if( strcasecmp(keyword, "final") == 0) token->type = COLLECTION;
    else
    {
        token->name =  keyword;
        token->type = FIELD;
    }
        token->lineNum = lineNum;
return token;
}

lexeme* lex(FILE *file)
{
    fp = file;
    skipWhiteSpace (fp);
    char ch;
    ch = fgetc(fp);
    //fprintf(stdout, "%c\n", ch);
    if (ch == EOF)
    {
        return newLex(END_OF_INPUT);
    }

    switch (ch)
    {
    case '(':
        return newLex(OPAREN);
    case ')':
        return newLex(CPAREN);
    case ',':
        return newLex(COMMA);
    case '-':
        ch = fgetc(fp);
        if (isdigit(ch))
        {
            ungetc(ch,fp);
            return newIntLex(fp, 1);
        }
    case '=':
        return newLex(EQUALS);
    case '.':
        return newLex(DOT);
    case ':':
        return newLex(COLON);
    case '<':
        ch = fgetc(fp);
        if(ch == '='){
            return newLex(LESSTHANEQUAL);
        }
        else
        if(ch == '>'){
            return newLex(NOT);
        }
        ungetc(ch, fp);
        return newLex(LESSTHAN);
    case '>':
        ch = fgetc(fp);
        if(ch == '='){
            return newLex(GREATERTHANEQUAL);
        }
        ungetc(ch, fp);
        return newLex(GREATERTHAN);
    case '[':
        return newLex(OSBRACKET);
    case ']':
        return newLex(CSBRACKET);
    default:
        if(isdigit(ch))
        {
         ungetc(ch,fp);
         return newIntLex(fp, 0);
        }
        else
        if (isalpha(ch))
        {
            ungetc(ch, fp);
            return newFieldLex(fp);
        }
        return newLex(UNKNOWN);
    }
}

void Display ( FILE *out, lexeme *l)
{
    char *s;
    switch(l->type)
    {
        case OPAREN:
            s = "Open Parenthesis";
            break;
        case CPAREN:
            s = "Close Parenthesis";
            break;
        case COMMA:
            s = "Comma";
            break;
        case END_OF_INPUT:
            s = "ENDOFINPUT";
            break;
        case EQUALS:
            s = "Equals =";
            break;
        case OSBRACKET:
             s = "SQUARE BRACKET - [";
            break;
        case CSBRACKET:
             s = "SQUARE BRACKET - ]";
            break;
        default:
            s = "UNKNOWN";
            break;
    }
  
     if (l->type == INTEGER)
        fprintf(out, "INTEGER: %d\n", l->integerVal);
    else if (l->type == FIELD)
        fprintf(out, "%s\n", l->name);  
    else if (l->type == LESSTHAN)
        fprintf(out, "LESSTHAN\n" );
    else if (l->type == GREATERTHANEQUAL)
        fprintf(out, "GREATERTHANEQUAL\n" );  
    else if (l->type == LESSTHANEQUAL)
        fprintf(out, "LESSTHANEQUAL\n" );
    else if (l->type == GREATERTHAN)
        fprintf(out, "GREATERTHAN\n" );    
    else if (l->type == PROGRAM)
        fprintf(out, "PROGRAM\n" );    
    else if (l->type == NEXT)
        fprintf(out, "NEXT\n" );
    else if (l->type == COND)
        fprintf(out, "CONDITIONAL\n" );
    else if (l->type == DOT)
        fprintf(out, "DOT\n" );    
    else if (l->type == NOT)
        fprintf(out, "NOT\n" );
    else if (l->type == SORT)
        fprintf(out,"Query | Sort | Count | Insert\n" );
    else if (l->type == QUERY)
        fprintf(out, "Query | Sort | Count | Insert\n" );    
    else if (l->type == COUNT)
        fprintf(out, "Query | Sort | Count | Insert\n" );
    else if (l->type == INSERT)
        fprintf(out,"Query | Sort | Count | Insert\n" );
    else if (l->type == COLLECTION)
        fprintf(out,"Collection Name: final\n" );
    else if (l->type == AND)
        fprintf(out,"AND\n" );
    else if (l->type == COLON)
        fprintf(out,"COLON\n" );
    else
    fprintf(out, "%s \n", s);
}



