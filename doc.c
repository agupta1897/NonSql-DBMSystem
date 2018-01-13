#include"scanner.h"
#include"doc.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<strings.h>
int sysid = 0;

int docIdFlag = 0;
attribute* newAttribute( char *Name, int value)
{
    attribute *token = malloc(sizeof(attribute));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    if(strcasecmp(Name, "DocID") == 0)
    {
        docIdFlag = 1;
    }
    token->Name = Name;
    token->value = value;
    token->next = NULL;
    return token;
}

doc* newDoc()
{
    doc *token = malloc(sizeof(doc));
    if (token == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    token->sysId = sysid++;
    token->versionNum = 1;
    token->next = NULL;
    token->prev = NULL;
    token->version = NULL;
    token->attri = newAttribute("sysid", sysid);
    token->totalVersions = 1;

   // Display(stdout, token);
   // fprintf(stdout,"-------\n");
    return token;
}
attribute* insertAttribute( attribute* head , attribute* newItem)
{
    attribute* x = head;
    newItem->next = NULL;
    while(head->next != NULL){
    head = head ->next ;
    }
    head->next = newItem;
    return x;
}

doc* funct( char* str)
{ 
    char *token;
    doc *ndoc = newDoc();
    int i = 0;
    char *name;
    int value;
    attribute *head = NULL;
    int flag = 0;
    token = strtok(str, " :");
    while( token != NULL ) 
    {
        
            if(i == 0)
            {
            name = token;
            //printf("%s* ", name);
            token = strtok(NULL, " :");
            i = 1;
            }
            else
            {
                value = atoi(token);
            // printf("%d, ", value);
                token = strtok(NULL, " :");
                i = 0;
                flag = 1;
            }
        if(flag == 1)
        {      
              if(head == NULL)
                {
                head = newAttribute(name, value);
                if(docIdFlag == 1)
                {
                    ndoc->docId = value;
                    docIdFlag = 0;
                }
                flag = 0;
                }
                else
                {
                    attribute *temp = newAttribute(name, value);
                    if(docIdFlag == 1)
                    {
                    ndoc->docId = value;
                    docIdFlag = 0;
                    }
                    insertAttribute(head, temp);
                    flag = 0;
                }
        }
    }

    ndoc->attri = insertAttribute(head, ndoc->attri);
    
   ndoc->attri = insertAttribute(ndoc->attri, newAttribute("vn", 1));
    return ndoc;
    
    // count++;
}


int returnAttribute1(attribute *item, char *str)
{
    while(item!= NULL)
    {
        if(strcmp(item->Name, str) == 0 )
        {
            return item->value;
        }
        item = item->next;
    }
    return -9999;
}


int displayAttribute( FILE *fp, attribute *x, attribute *list, int xyz)
{
 if(xyz == 0)
 {
    if(list != NULL)
    {
    int flag = 0;
    while(list != NULL)
    {
    if( returnAttribute1(x,list->Name) != -9999) 
       {  
           flag = 1;
           fprintf(fp, "%s:%d ", list->Name, returnAttribute1(x, list->Name)); 
       }
       list = list->next;
    }
    
    return flag;
    }
    else
    {
            while( x != NULL)
            {
                fprintf (fp, "%s:", x->Name);
                fprintf (fp, "%d ", x->value );
                x = x->next;
            }
           
            return 1;
        }
    }
    else
    {
            while( x != NULL)
            {
                if(strcmp(x->Name, "sysid")!=0 && strcmp(x->Name, "vn")!=0 )
                {
                fprintf (fp, "%s:", x->Name);
                fprintf (fp, "%d ", x->value );
                }
                x = x->next;
            }
           
            return 1;
    }
    
}

int shoulddisplayAttribute( FILE *fp, attribute *x, attribute *list, int xyz)
{
 if(xyz == 0)
 {
    if(list != NULL)
    {
    int flag = 0;
    while(list != NULL)
    {
    if( returnAttribute1(x,list->Name) != -9999) 
       {  
           flag = 1;
           //fprintf(fp, "%s:%d ", list->Name, returnAttribute1(x, list->Name)); 
       }
       list = list->next;
    }
    
    return flag;
    }
    else
    {
            // while( x != NULL)
            // {
            //     fprintf (fp, "%s:", x->Name);
            //     fprintf (fp, "%d ", x->value );
            //     x = x->next;
            // }
           
            return 1;
        }
    }
    else
    {
            while( x != NULL)
            {
                if(strcmp(x->Name, "sysid")!=0 && strcmp(x->Name, "vn")!=0 )
                {
                fprintf (fp, "%s:", x->Name);
                fprintf (fp, "%d ", x->value );
                }
                x = x->next;
            }
           
            return 1;
    }
    
}


int findAttribute1(attribute *item, char *str)
{
    while(item!= NULL)
    {
        if(strcmp(item->Name, str) == 0 )
        {
            return 1;
        }
        item = item->next;
    }
    return 0;
}

void displayData(FILE* fp, doc *head)
{
    while(head!= NULL )
    {
        //fprintf(fp, "DOC_ID: %d\tSYSTEM_ID: %d\tVn: %d\t Vt: %d \t", head->docId, head->sysId, head->versionNum, head->totalVersions);
        
        displayAttribute(fp, head->attri, NULL, 1);
        fprintf(fp, "%s", "\n");
        displayData(fp, head->version);
        
        head = head->next;
    }
}

doc* insertdoc( doc* head , doc* newItem)
{
    doc* x = head;
    newItem->next = NULL;
    while(head->next != NULL){
    head = head ->next ;
    }
    head->next = newItem;
    return x;
}

attribute* updateVersionList( attribute *list, int version)
{
    attribute *x = list;
    while(list!= NULL)
    {
        if(strcmp(list->Name, "vn") == 0)
        {
            list->value = version;
        }
        list = list->next;
    }
    return x;
}

void versionInsert( doc *old, doc* new){

    if(old->version == NULL)
    {
        old->version = new;
        old->totalVersions++;
        new->versionNum = old->versionNum + 1;
        new->attri = updateVersionList(new->attri, new->versionNum);
    }
    else
    {
        // new->version = old->version;
        // old->version = new;
         old->totalVersions++;
        while(old->version != NULL)
        {
           
            old = old->version;

        }
        old->version = new;
        new->prev = old;
        new->versionNum = old->versionNum + 1;
        new->attri = updateVersionList(new->attri, new->versionNum);
    }
}

int versionChecker( doc *head, doc* new1)
{
    while(head!= NULL)
    {
        if (new1->docId == head->docId)
        {
            versionInsert(head, new1);
            return 1;
        }
        head = head->next;
    }
    return 0;
}

 doc* readFile( FILE *fp)
 {
    char *t;
    doc* head = NULL;
    doc* temp;
    t = readLine(fp);
    // while(!feof(fp) || strlen(t)< 2)
    // {
    //     t = readLine(fp);
    // }
    while(!feof(fp))
    {
        if(head == NULL)
        {
        
        head = funct(t);
        
        }
        else
        {
            
            temp = funct(t);
            if(versionChecker(head, temp) == 0)
            {
            insertdoc (head, temp);
            }
        }
        t = readLine(fp);
        // while(!feof(fp) || strlen(t)< 2)
        // {
        // t = readLine(fp);
        // }

    }
   // head->totalsysId = sysid;
   sysid = 0;
    return head;
 }



