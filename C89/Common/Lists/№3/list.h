#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

// One-linked list
typedef struct node
{
	char* data;
	struct node* next;
}NODE;


void  ListPrint(NODE* head);
void  ListDestroy(NODE* head);
NODE* ListAppend(NODE* head, char* data);

void ListCheckEntry(NODE* main, NODE* input);

#endif