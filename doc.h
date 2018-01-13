#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef __DOC_INCLUDED__
#define __DOC_INCLUDED__


typedef struct attribute
    {
        char *Name;
        int value;
        struct attribute *next;
    } attribute;

typedef struct doc
    {
        int docId;
        int versionNum;
        int sysId;
        //int totalsysId;
        struct attribute* attri;
        struct doc* next;
        struct doc* version;
        struct doc* prev;
         int totalVersions;
         

    } doc;

extern void displayData(FILE *fp, doc* head);
extern int displayAttribute( FILE *fp, attribute *x, attribute *list, int xyz);
extern int shoulddisplayAttribute( FILE *fp, attribute *x, attribute *list, int xyz);
extern attribute* newAttribute( char *name, int value);
extern doc* newDoc();
extern doc* insertdoc( doc* head, doc* newItem);
extern int versionChecker( doc *head, doc* new1);
extern attribute* insertAttribute(attribute* head , attribute* newItem);
extern doc* readFile( FILE *fp);
#endif