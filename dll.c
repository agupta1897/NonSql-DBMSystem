/*
Developed by: Amber Gupta
Supervision of : Dr. John Lusth 

This is a source file for dll.h. It defines the functionalities reuired for the Doubly Linked List.
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dll.h"
#include "doc.h"

  dll *newDLL() //constructor
	    {
        dll *items = malloc(sizeof(dll));
        if (items == 0)
            {
            fprintf(stderr,"out of memory");
            exit(-1);
            }
        items->head = 0;
        items->tail = 0;
        items->size = 0;
       
        return items;

        }


	
    void insertDLL(dll *items,int index, doc *value){ //inserts a node at the provided index. 
		dllnode *newnode = malloc(sizeof(dllnode));
        newnode->next=NULL;
		newnode->prev=NULL;
        newnode->value=value;
         if(items->size==index)
         {
             if(items->size==0) 
             {
                 items->head=newnode;
                 items->tail=newnode;
             }
             else
             {
                 newnode->prev=items->tail;
				 items->tail->next=newnode;
                 items->tail=newnode;
                 
             }
         }
        else
        {
            if(index==0)
            {
				items->head->prev=newnode;
                newnode->next=items->head;
                items->head=newnode;
            }
            else {
                 dllnode *temp;
            int tempindex=0;
            temp=items->head;
            while(tempindex!=index-1)
            {
                temp=temp->next;
                tempindex++;
            }
				newnode->prev=temp;
               newnode->next=temp->next;
                temp->next=newnode;
				temp=newnode->next;
				temp->prev=newnode;
            
            }
        }
        items->size+=1;
	}
	     //returns a generic value and removes the node at the provided index value
    // void *removeDLL(dll *items,int index){
	// 	  void *rvalue=0;
    //     if(index==0)
    //     {
    //         if(items->size==1)
    //         {
    //             rvalue=getDLL(items,index);
    //               items->head=0;
    //             items->tail=0;
    //              items->size-=1;
    //             return rvalue;
    //         }
    //         else
                  
    //         items->head=items->head->next;
 	// 		items->head->prev=0;
	// 		items->size-=1;
    //         return rvalue;
            
    //     }
    //     else if(index==items->size-1)
    //     {
    //           rvalue=items->tail->value;
    //         dllnode *temp;
    //       temp=items->tail->prev;
			
    //         temp->next=0;
    //         items->tail=temp;
    //          items->size-=1;
    //         return rvalue;
    //     }
    //     else if(index<(items->size/2))
    //     {
    //           rvalue=getDLL(items,index);
    //         dllnode *temp;
    //         int tempindex=0;
    //         temp=items->head;
    //         while(tempindex!=index-1)
    //         {
    //             temp=temp->next;
    //             tempindex++;
    //         }
            
    //         temp->next=temp->next->next;
    //         dllnode* cotemp;
	// 		cotemp= temp->next;
	// 		cotemp->prev=temp;
			
	// 		items->size-=1;
    //         return rvalue;
    //     }
	// 	else
	// 	{
	// 		dllnode *temp;
	// 		int tempindex = items->size;
	// 		tempindex--;
	// 		temp=items->tail;
	// 		while(tempindex!=index)
	// 		{
	// 			temp=temp->prev;
	// 			tempindex--;
	// 		}
	// 		rvalue=temp->value;
	// 		dllnode *cotemp;
	// 		cotemp=temp->prev;
	// 		cotemp->next=temp->next;
	// 		cotemp=cotemp->next;
	// 		cotemp->prev=temp->prev;
	// 		items->size-=1;
	// 		return rvalue;
			
	// 	}
	// 	return rvalue;
	// }       
    // void unionDLL(dll *recipient,dll *donor){
	// 	if(recipient->size==0)
	// 	{
	// 		recipient->head=donor->head;
	// 		recipient->tail=donor->tail;
	// 		recipient->size=donor->size;
	// 		donor->head=0;
    //  	    donor->size=0;
    //  	    donor->tail=0;
	// 		return;
	// 	}
	// 	else if(donor->size==0)
	// 	{
	// 		return;
	// 	}
	// 	else
	// 	{
	// 	  recipient->tail->next=donor->head;
	// 	 donor->head->prev=recipient->tail;
    //      recipient->tail=donor->tail;
    //      recipient->size +=donor->size;
    //      donor->head=0;
    //      donor->size=0;
    //      donor->tail=0;
	// 	return;
	// 	}
	// }//merge two lists into one


    // void *getDLL(dll *items,int index){
		
    //      if(index==0)
    //      {
    //          return items->head->value;
    //      }
    //      else if(index==items->size-1)
    //      {
    //          return items->tail->value;
    //      }
    //      else
         
    //     {
    //         dllnode *temp;
    //         int tempindex=0;
    //         temp=items->head;
    //         while(tempindex!=index)
    //         {
    //             temp=temp->next;
    //             tempindex++;
    //         }
    //         return temp->value;
    //      }
    //  }	//get the value at the index
    

int sizeDLL(dll *items){ //returns the size of the dll
	return items->size;
}



void display1( FILE *fp, doc* head, attribute *list)
{
    if(list!= NULL)
    {
    if(shoulddisplayAttribute(fp, head->attri, list, 0) != 0)
    {
        displayAttribute(fp, head->attri, newAttribute("vn", 0), 0);
        displayAttribute(fp, head->attri, list, 0);
        // if( displayAttribute(fp, head->attri, list, 0) == 0)
        // {
        // printf("SHOULD HAVE NOT BEEN PRINTEED"); 
        // }
     fprintf(fp, "\n");
    }
    }
    else
    {
        displayAttribute(fp, head->attri, newAttribute("vn", 0), 0);
        displayAttribute(fp, head->attri, newAttribute("sysid", 0), 0);
        displayAttribute(fp, head->attri, list, 1);
        fprintf(fp, "\n");
    }

    
}

void displayDLL(FILE *fp,dll *items, attribute *list)
	 	{
        int size= items->size;
         dllnode *node = malloc(sizeof(dllnode));
         node = items->head;
         
         while(size>0)
         {
             display1(fp, node->value, list);
             node=node->next;
             size-=1;   
            }
            fprintf(fp, "\n");
       
         return;
        }
        


  