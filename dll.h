/*
Developed by: Amber Gupta
Supervision of : Dr. John Lusth 

This is a header file for dll.h. It declares the functionalities required for the needed operations on Doubly Linked Lists.
*/
#include"doc.h"
#ifndef __DLL_INCLUDED__
    #define __DLL_INCLUDED__
#include<stdio.h>
    typedef struct dllnode
        {
        doc *value;
        struct dllnode *next;
		struct dllnode *prev;
        } dllnode;

    typedef struct dll 
        {
        struct dllnode *head;
        struct dllnode *tail;
        
        int size;
       // void (*display)(FILE *,void *);
        } dll;

  dll *newDLL();            //constructor
    void insertDLL(dll *items,int index,doc *value); //stores a generic value
   // void *removeDLL(dll *items,int index);            //returns a generic value
   // void unionDLL(dll *recipient,dll *donor);         //merge two lists into one
   // void *getDLL(dll *items,int index);               //get the value at the index
    int sizeDLL(dll *items); 
    void displayDLL(FILE *,dll *items, attribute *list);

    #endif

