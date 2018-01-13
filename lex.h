#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<strings.h>
#ifndef __LEX_INCLUDED__
#define __LEX_INCLUDED__
typedef enum
{
COLLECTION,
DOT,
PROGRAM,
QUERY,
COUNT,
INSERT,
SORT,
AND,
LESSTHAN,
GREATERTHAN,
GREATERTHANEQUAL,
LESSTHANEQUAL,
EQUALS,
INTEGER,
FIELD,
OPAREN,
OSBRACKET,
COMMA,
CSBRACKET,
CPAREN,
COND,
END_OF_INPUT,
NOT,
UNKNOWN,
COLON,
NEXT
} types;

typedef struct lexeme
    {
        char* name;
        int integerVal;
        types type;
        int lineNum;
        struct lexeme *right;
        struct lexeme *left;
        struct doc* head;
    }   lexeme;
extern lexeme *newLex (types type);
extern lexeme *newFieldLex(FILE *fp);
extern void Display (FILE* fp, lexeme *l);
extern lexeme *lex (FILE *fp);

#endif