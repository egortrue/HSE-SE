#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

// One-linked list
typedef struct node
{
	int data;
	struct node* next;
}NODE;


void  ListPrint         (NODE* head);
void  ListDestroy       (NODE* head);
NODE* ListAppend        (NODE* head, int data);

NODE* ListCreateDiff(NODE* L1, NODE* L2);


#endif