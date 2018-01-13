#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifndef __RECOGNIZE_INCLUDED__
#define __RECOGNIZE_INCLUDED__

extern lexeme* recognize( FILE *fp );
extern void printTree(FILE *fp, lexeme* tree, int flag);
#endif