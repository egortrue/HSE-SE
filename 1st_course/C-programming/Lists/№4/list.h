#ifndef LIST_H
#define LIST_H

// One-linked list
typedef struct node
{
	unsigned char data;
	struct node* next;
}NODE;


void  ListPrint(NODE* head);
void  ListDestroy(NODE* head);
NODE* ListAppend(NODE* head, unsigned char data);

void ListSeparateNumsAndLetters(NODE* list, NODE** nums, NODE** letters);

#endif