#include"scanner.h"
#include"lex.h"
#include"recognizer.h"
#include"doc.h"
#include"dll.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE* writeFile;
FILE* query1;

// int numofAlphanumeric( char*s)
// {
//     if(s!=NULL)
//     {
//         int x = strlen(s);
//         for
//     }
    
// }

void writeQueryToFile()
{
    //fprintf(stdout, "HERE\n");
    char* s = readLine(query1);
    while(strlen(s) < 10)
    {
        if(feof(query1))
        {
            return;
        }
        s = readLine(query1);
   
    }

     fprintf(writeFile,"%s\n",s);


}

int findAttribute(attribute *item, char *str)
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

//flag = 0 ALL VERSION
//flag = -1 LATEST VERSION
//else Version Range

// while(head!= NULL )
//     {
//         fprintf(fp, "DOC_ID: %d\tSYSTEM_ID: %d\tVn: %d\t", head->docId, head->sysId, head->versionNum);
        
//         displayAttribute(fp, head->attri);
//         displayData(fp, head->version);

//         head = head->next;
//     }

int countVersion(doc *ptr, char *str, int flag)
{
    int count = 0;
 
    if(flag == 0)
    {
        while(ptr->next!= NULL)
        {
        ptr = ptr->next;
        }
        if (findAttribute(ptr->attri, str) == 1)
        {
           // fprintf(stdout, "THIS IS IT MATCH\n");
            //fprintf(stdout, "%d", ptr->versionNum);
            count++;
        }
            
        return count;
    }
     else
   {
    while(ptr!= NULL)
        {
        if (findAttribute(ptr->attri, str) == 1)
            {
               // fprintf(stdout, "THIS IS IT MATCH\n");
                count++;
            }
            
        ptr = ptr->next;
        }
        return count;
    }
}


//flag = 0 ALL VERSION
//flag = -1 LATEST VERSION
//else Version Range

int countFunc(doc *ptr, char *str, int flag)
{
    int count = 0;
    doc* cpt;
   if(flag == 0 )
   {
    while(ptr!= NULL)
        {
        if (findAttribute(ptr->attri, str) == 1)
            {
                //fprintf(stdout, "THIS IS IT MATCH\n");
                count++;
            }
                cpt = ptr->version;
            while(cpt!= NULL)
            {
                   count = count + countVersion( cpt, str, flag);
                    cpt= cpt->version;
            }
        ptr = ptr->next;
        }
        return count;
    }

    if(flag == -1)
    {
        while(ptr!= NULL)
        {
            if(ptr->version== NULL)
            {
                if (findAttribute(ptr->attri, str) == 1)
                {
                    //fprintf(stdout, "THIS IS IT MATCH\n");
                    count++;
                }
            }
            else
            {
                cpt = ptr->version;
                while(cpt->version != NULL)
                {
                    
                        cpt= cpt->version;
                }
                count = count + countVersion( cpt, str, flag);
            }
        ptr = ptr->next;
        }
        return count;
    }
    else
    {
        while(ptr!= NULL)
        {
            if(ptr->totalVersions <= flag)
            {
                if (findAttribute(ptr->attri, str) == 1)
                {
                    //fprintf(stdout, "THIS IS IT MATCH\n");
                    count++;
                }
                cpt = ptr->version;
                while(cpt!= NULL)
                {
                    count = count + countVersion( cpt, str, flag);
                        cpt= cpt->version;
                }
            }
            else
            {
                int x = ptr->totalVersions - flag;
                 cpt = ptr->version;
                while(cpt!= NULL)
                {
                    if(cpt->versionNum > x)
                    {
                    count = count + countVersion( cpt, str, flag);
                    }
                   cpt= cpt->version;
                }
            }
        ptr = ptr->next;
        }
        return count;
    }
}


lexeme* evalCount(lexeme *tree)
{
    writeQueryToFile();    
  // fprintf(stdout, "HERE\n");
     FILE* fp = fopen("readfile.txt", "r");
    doc* head = readFile(fp);
    fclose(fp);
    // displayData(stdout, head);
    char* fieldname = tree->left->name;
    int versions = -1;
    if(tree->right!= NULL)
    {
        versions= tree->right->integerVal;
    }

    if(versions==-1)
    {
        
     fprintf(writeFile,"Count_%s: %d\n\n", fieldname, countFunc(head, fieldname,-1 ));
    }
   else
   if(versions == 0)
   fprintf(writeFile,"Count_%s: %d\n\n", fieldname, countFunc(head, fieldname, versions));
   else
   fprintf(writeFile,"Count_%s: %d\n\n",fieldname, countFunc(head, fieldname,versions ));

   return tree;

}

dll* evaluateSort(char *name, doc* ptr, int flag)
{
  
    doc* cpt;
    
    dll* list = newDLL();
    if(flag == -1)
    {

        while(ptr!= NULL)
            {
                if(ptr->totalVersions == 1)
                {
                    if (findAttribute(ptr->attri, name) == 1)
                    {
                        insertDLL(list, sizeDLL(list), ptr);
                    }
                }
                else
                {
                    cpt = ptr->version;
                    while(cpt->version!= NULL)
                    {
                        cpt= cpt->version;
                    }
                    if (findAttribute(cpt->attri, name) == 1)
                    {
                        insertDLL(list, sizeDLL(list), cpt);
                    }
                        
                }
            
            ptr = ptr->next;
        }
        return list;
        
    }
    else
    if(flag == 0)
    {
         while(ptr!= NULL)
            {
               
                if (findAttribute(ptr->attri, name) == 1)
                    {
                        insertDLL(list,sizeDLL(list), ptr);
                    }

                cpt = ptr->version;
                while(cpt!= NULL)
                {
                    if (findAttribute(cpt->attri, name) == 1)
                    {
                        insertDLL(list, sizeDLL(list), cpt);
                    }
                    cpt= cpt->version;
                }
            ptr = ptr->next;
            }
        return list;
    }
    else
    {
        
            while(ptr!= NULL)
            {
                if(ptr->totalVersions <= flag)
                {
                   // printf("HERE\n");
                    if (findAttribute(ptr->attri, name) == 1)
                    {
                        insertDLL(list,sizeDLL(list), ptr);
                    }
                    cpt = ptr->version;
                     while(cpt!= NULL)
                    {
                    if (findAttribute(cpt->attri, name) == 1)
                    {
                        insertDLL(list, sizeDLL(list), cpt);
                    }
                        cpt= cpt->version;
                    }
                }
                else
                {
                   // printf("HERE%d\n", flag);
                    int x = ptr->totalVersions - flag;
                    cpt = ptr->version;
                    while(cpt!= NULL)
                    {
                        if(cpt->versionNum > x)
                        {
                            if (findAttribute(cpt->attri, name) == 1)
                            {
                             insertDLL(list, sizeDLL(list), cpt);
                             }
                        }
                    cpt= cpt->version;
                    }
                }
            ptr = ptr->next;
            }
        return list ;    
    }
}


int returnAttributeVal(attribute *item, char *str)
{
    while(item!= NULL)
    {
        if(strcmp(item->Name, str) == 0 )
        {
            return item->value;
        }
        item = item->next;
    }
    return 0;
}


int comparison( doc* first, doc * second, char *s)
{
   if (returnAttributeVal(first->attri, s) < returnAttributeVal(second->attri, s))
   {
       return 1;
    }
    else
    return 0;
}

void swap(dllnode *a, dllnode*b)
{
    doc* temp = a->value;
    a->value= b->value;
    b->value = temp;
}
dll* sorter( dll* list, char *s)
{
    if(sizeDLL(list) < 2 )
    {
        return list;
    }
    else
    {
        int swapped;
       // dllnode *ptr1;
        dllnode *lptr = NULL;
        //dllnode *ptr;
        dllnode *ptr1;
        do
        {
            swapped = 0;
            ptr1 = list->head;
        
        while(ptr1->next !=lptr)
        {
            
            if(comparison(ptr1->value, ptr1->next->value, s)== 0)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
            lptr = ptr1;
        }
        while(swapped);
    }
    return list;
    }
    


lexeme *evalSort(lexeme* tree)
{
 writeQueryToFile();
    
    FILE *fp1 = fopen("readfile.txt", "r");
    doc* head = readFile(fp1);
    char *s = tree->left->name;
    int versions = -1;
    if(tree->right!= NULL)
    {
        versions= tree->right->integerVal;
    }

    dll* list = evaluateSort(s, head,  versions);
    //displayDLL(stdout, list, NULL);

    list = sorter(list, s);
    //printf("\nSORTED\n\n");
    displayDLL(writeFile, list, NULL);
    fclose(fp1);
    return tree;
}




lexeme *eval(lexeme* tree);

dll* evalCondition(lexeme* tree, int flag, dll* result, doc* head);
int firstTime = 0;

void projectionDisplay( lexeme *tree, dll* list)
{
    attribute *head = NULL ;
   // attribute *ptr = NULL;
    if(tree->left != NULL)
    {
        head = newAttribute(tree->left->name, 0);
        //ptr = head;
    
    tree= tree->left;
    while(tree->left!= NULL)
    {
        head = insertAttribute(head, newAttribute(tree->left->name, 0));
        tree= tree->left;
    }
    }
    displayDLL( writeFile, list, head);
}

dll* makeDllNoCondition( doc* ptr, int flag)
{
    doc* cpt;
    dll* list = newDLL();
    if(flag == -1)
    {
        while(ptr!= NULL)
            {
                if(ptr->totalVersions == 1)
                {
                        insertDLL(list, 0, ptr); 
                }
                else
                {
                    cpt = ptr->version;
                    while(cpt->version!= NULL)
                    {
                        cpt= cpt->version;
                    }
                    insertDLL(list, 0, cpt);   
                }
            
            ptr = ptr->next;
        }
        return list;
        
    }
    else
    if(flag == 0)
    {
         while(ptr!= NULL)
            { 
                insertDLL(list, 0, ptr);
                cpt = ptr->version;
                while(cpt!= NULL)
                {
                    insertDLL(list, 0, cpt);
                    cpt= cpt->version;
                }
            ptr = ptr->next;
            }
        return list;
    }
    else
    {
        while(ptr!= NULL)
        {
            if(ptr->totalVersions <= flag)
            {
                // printf("HERE\n");
                insertDLL(list, 0, ptr);
                cpt = ptr->version;
                    while(cpt!= NULL)
                {
                    insertDLL(list, 0, cpt);
                    cpt= cpt->version;
                }
            }
            else
            {
                // printf("HERE%d\n", flag);
                int x = ptr->totalVersions - flag;
                cpt = ptr->version;
                while(cpt!= NULL)
                {
                    if(cpt->versionNum > x)
                    {
                        insertDLL(list, 0, cpt);
                    }
                cpt= cpt->version;
                }
            }
        ptr = ptr->next;
        }
    return list ;    
    }
}


int isAttributePresent( attribute *checkfrom, attribute *list)
{
    attribute *ptr = checkfrom;
    attribute *cpt = list;

     while(cpt!= NULL)
        {
            if (findAttribute(ptr, cpt->Name) == 1)
            {
               return 1;
            }
            
        cpt = cpt->next;
    }
    return 0;
}

dll* finalConditionAnswer( dll* list1, attribute* attriList)
{
    dll* result = newDLL();
    dllnode *ptr = list1->head;
    while(ptr!= NULL)
    {
        if(isAttributePresent(ptr->value->attri, attriList) == 1)
        {
           insertDLL(result, sizeDLL(result), ptr->value);
        }
        ptr = ptr->next;
    }
    return result;
}


 attribute* conditionAttributes(lexeme* tree)
 {
     attribute *head = newAttribute(tree->left->name, 0);
     tree = tree->right->right;
     while(tree!= NULL)
     {
         insertAttribute(head, newAttribute(tree->left->name, 0));
         tree = tree->right->right;
        }
        return head;
 }



lexeme* evalQuery( lexeme* tree)
{
   // fprintf(stdout, "HERE\n");
    writeQueryToFile();   
     FILE* fp = fopen("readfile.txt", "r");
    doc* head = readFile(fp);
    int versions = -1;
    firstTime = 0;
    dll *result = NULL;
    dll *result1 = NULL;
    attribute *condAttriList;
    if(tree->left != NULL)
    {
        condAttriList= conditionAttributes(tree->left);
       // displayAttribute(stdout, condAttriList, NULL, 1);
        if(tree->right->right!= NULL)
        {
                versions= tree->right->right->integerVal;
               // printf("%d\n", versions);
               result =  evalCondition(tree->left, versions, result, head);
        }
         else
         result = evalCondition(tree->left, -1, result, head);
            // 
            //fprintf(stdout, "EFWEWFE\n");
            result1 = finalConditionAnswer(result, condAttriList);
             projectionDisplay(tree->right, result1);
    }
    else
    {
        if(tree->right->right!= NULL)
        {
                versions= tree->right->right->integerVal;
               // printf("%d\n", versions);
               result = makeDllNoCondition(head, versions );
        }
         else
         result = makeDllNoCondition(head, versions );
            // 
             projectionDisplay(tree->right, result);
            
    }
return tree;
}


int findValueOp(attribute *item, char *str, int value, types type, int isOnlyField)
{
    while(item!= NULL)
    {
        if(strcmp(item->Name, str) == 0 )
        {
            //printf("\n**********");
            switch(type)
            {
                
                case GREATERTHAN:
                    if(item->value > value)
                    {
                        return 1;
                    }
                    else
                    return 0;
                case EQUALS:
                    if(item->value == value)
                    {
                        return 1;
                    }
                    else
                    return 0;
                case LESSTHAN:
                    if(item->value < value)
                    {
                        return 1;
                    }
                    else
                    return 0;
                case NOT:
                    if(item->value != value)
                    {
                        return 1;
                    }
                    else
                    return 0;
                case GREATERTHANEQUAL:
                    if(item->value >= value)
                    {
                        return 1;
                    }
                    else
                    return 0;
                case LESSTHANEQUAL:
                    if(item->value <= value)
                    {
                        return 1;
                    }
                    else
                    return 0;
                    default:
                    return 0;
            }
            
        }
        item = item->next;
    }
    if(isOnlyField == 1)
    {
        return 0;
    }
    else
    return 1;
}




dll* evaluateOperations(char *name, int value, doc* ptr, types type, int flag, int isOnlyField)
{
    
    doc* cpt;
    
    dll* list = newDLL();
    if(flag == -1)
    {

        while(ptr!= NULL)
            {
                if(ptr->totalVersions == 1)
                {
                    if (findValueOp(ptr->attri, name, value, type, isOnlyField) == 1)
                    {
                        insertDLL(list, 0, ptr);
                    }
                }
                else
                {
                    cpt = ptr->version;
                    while(cpt->version!= NULL)
                    {
                        cpt= cpt->version;
                    }
                    if (findValueOp(cpt->attri, name, value, type, isOnlyField) == 1)
                        {
                        
                            insertDLL(list, 0, cpt);
                        }
                        
                }
            
            ptr = ptr->next;
        }
        return list;
        
    }
    else
    if(flag == 0)
    {
         while(ptr!= NULL)
            { 
                if (findValueOp(ptr->attri, name, value, type,isOnlyField) == 1)
                {
                    insertDLL(list, 0, ptr);
                }

                cpt = ptr->version;
                while(cpt!= NULL)
                {
                    if (findValueOp(cpt->attri, name, value, type, isOnlyField) == 1)
                    {
                        insertDLL(list, 0, cpt);
                    }
                    cpt= cpt->version;
                }
            ptr = ptr->next;
            }
        return list;
    }
    else
    {
        while(ptr!= NULL)
        {
            if(ptr->totalVersions <= flag)
            {
                // printf("HERE\n");
                if (findValueOp(ptr->attri, name, value, type, isOnlyField) == 1)
                {
                    insertDLL(list, 0, ptr);
                }
                cpt = ptr->version;
                    while(cpt!= NULL)
                {
                if (findValueOp(cpt->attri, name, value, type, isOnlyField) == 1)
                    {
                    
                        insertDLL(list, 0, cpt);
                    }
                    cpt= cpt->version;
                }
            }
            else
            {
                // printf("HERE%d\n", flag);
                int x = ptr->totalVersions - flag;
                cpt = ptr->version;
                while(cpt!= NULL)
                {
                    if(cpt->versionNum > x)
                    {
                        if (findValueOp(cpt->attri, name, value, type, isOnlyField) == 1)
                        {
                            insertDLL(list, 0, cpt);
                        }
                    }
                cpt= cpt->version;
                }
            }
        ptr = ptr->next;
        }
    return list ;    
    }
}



dll* evaluateOperations2(char *name, int value, dll* ptr, types type)
{
    dll *list = newDLL();
    dllnode *cpt = ptr->head;
    while(cpt!= NULL)
    {
        if (findValueOp(cpt->value->attri, name, value, type, 0) == 1)
            {
                insertDLL(list, list->size, cpt->value);
            }
            cpt= cpt->next;
        }
        return list;
}



dll* evalCondition(lexeme* tree, int flag, dll* resultList, doc* head)
{
    
    char *field = tree->left->name;
    int value = tree->right->left->integerVal;
    int isOnlyField = 0;
    if(tree->right->right == NULL)
    {
        if(firstTime == 0)
        {
            isOnlyField = 1;
        }
    }
    //printf("%d", isOnlyField);
    
    if( firstTime == 0)
    {
     firstTime = 1;
     resultList = evaluateOperations(field, value, head, tree->right->type, flag, isOnlyField);
     //displayDLL(stdout,resultList, NULL);
    }
    else
    {
        resultList = evaluateOperations2(field, value, resultList, tree->right->type);
    }
    
    if(tree->right->right != NULL)
    {
        resultList = evalCondition(tree->right->right, flag, resultList, head);
       // displayDLL(stdout,resultList, NULL);
    }

   // doc* x =  resultList->head->value;
   // displayAttribute(stdout, x->attri);    
    return resultList;
}

int docId = 0;

attribute* returnAttributeList(lexeme *tree)
{
    attribute* head;
     if(tree->right != NULL)
        {
            if(strcasecmp(tree->right->name, "DocID") == 0)
                {
                    docId = tree->right->left->integerVal ;
                }
        head = newAttribute(tree->right->name, tree->right->left->integerVal );
        tree= tree->right;
    while(tree->right!= NULL)
        {
            if(strcasecmp(tree->right->name, "DocID") == 0)
                {
                    docId = tree->right->left->integerVal ;
                }
        head = insertAttribute(head, newAttribute(tree->right->name,tree->right->left->integerVal));
        tree= tree->right;
        }
    }
    return head;
}


lexeme *evalInsert(lexeme *tree)
{
 writeQueryToFile();
    
    FILE *fp1 = fopen("readfile.txt", "r");
    doc* head = readFile(fp1);
        
    attribute* attributeList = returnAttributeList(tree);
    doc* newDocument = newDoc();
    doc* temp;
    newDocument->docId = docId;
   
    newDocument->attri = insertAttribute(attributeList, newAttribute("sysid", newDocument->sysId  ));
    newDocument->attri = insertAttribute(attributeList, newAttribute("vn", 1 ));
    if(head == NULL)
        {
        head = newDocument;
        }
        else
        {
            temp = newDocument; 
            if(versionChecker(head, temp) == 0)
            {
            head = insertdoc (head, temp);
            }
        }
         fclose(fp1);
        //displayData(fp1, head);
        FILE *fp2 =  fopen("readfile.txt", "w+");
        displayData(fp2,head);
        displayData(writeFile, head);
        fclose(fp2);
    return tree;
}


lexeme *eval(lexeme* tree)
{
    
    switch(tree->type)
    {
        case NEXT:

        while(tree->right != NULL)
        {
            eval(tree->left);
            tree= tree->right;
        }
        return eval(tree->left);

        case COUNT:
            return evalCount(tree);
        
        case INSERT:
            return evalInsert(tree);

        case QUERY:
            return evalQuery(tree);

        case SORT:
            return evalSort(tree);
        default:     
        return tree; 
    }
}





int main (int argc, char** argv)
{
    if(argc == 0)
    exit(1);
    FILE *fp1 = fopen("data.txt","r");
    doc* head = readFile(fp1);
    FILE *fp2 = fopen("readfile.txt", "w+");
    displayData(fp2, head);
    fclose(fp2);
    fclose(fp1);

    // fp1 = fopen("readfile.txt","r");
    // head = readFile(fp1);
    // displayData(stdout, head);
    // fclose(fp1);
    FILE *fp = fopen("query.txt","r");
    
   // writeFile = fopen()
    lexeme *tree = recognize(fp);
    fclose(fp);
  
    query1 = fopen("query.txt","r");
    writeFile = fopen("agupta18.txt","w+");
    //printTree(stdout, tree->right, 2);
    //printTree(stdout, tree->right, 2);
    eval(tree->right);
    fclose(query1);
    fclose(writeFile);
    return 0;
}
