#include<stdio.h>
#include<string.h>

#include "doublylinkedlist.h"

void addStudent(Node *pointer, Student data) {
     while(pointer->next!=NULL) {
          pointer = pointer -> next;
     }
     pointer->next = (Node *)malloc(sizeof(Node));
     (pointer->next)->prev = pointer;
     pointer = pointer->next;
     pointer->data = data;
     pointer->next = NULL;
}

Student findStudent(Node *pointer, int key) {
     Student temp;
     temp.id = -1;
     
     pointer =  pointer -> next; 
     while(pointer!=NULL) {
          if(pointer->data.id == key){ 
               return pointer->data;
          }
          pointer = pointer -> next;
     }
     return temp;
}
void deleteStudent(Node *pointer, int id) {
     while(pointer->next!=NULL && (pointer->next)->data.id != id) {
          pointer = pointer -> next;
     }
     if(pointer->next==NULL) {
          printf("Student is not present in the list\n");
          return;
     }
     Node *temp;
     temp = pointer -> next;
     pointer->next = temp->next;
     temp->prev =  pointer;
     free(temp);
     return;
}

int getSize(Node *pointer){
     int count = 0;
     while(pointer->next!=NULL) {
          pointer = pointer -> next;
          count++;
     }
     return count;
}
