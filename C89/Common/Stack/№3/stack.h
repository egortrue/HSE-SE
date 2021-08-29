#ifndef STACK_H
#define STACK_H

typedef struct node  // 4 bytes + 1 bytes => 8 bytes
{
	struct node* next;
	unsigned char data;
}NODE;

void StackPush(char data);
void StackPop();

int  StackIsEmpty();
char StackGet();

void StackPrint();
void StackDestroy();

// LIST AS ADDITION FOR EXAMPLE ¹3
void ListPrint(NODE* head);
void ListDestroy(NODE* head);
NODE* ListAppend(NODE* head, char data);
char ListGetData(NODE* head, int level);

#endif
