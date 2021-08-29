#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct node  // 4 bytes + 1 bytes => 8 bytes
{
	struct node* next;
	unsigned char data;
}NODE;

NODE* top = NULL;

void StackPrint()
{
	NODE* tmp = top;
	while (tmp != NULL)
	{
		printf("%c | ", tmp->data);
		tmp = tmp->next;
	}
	printf("\n");
}

void StackPush(char data)
{
	// Create temp object
	NODE* tmp = (NODE*)malloc(sizeof(NODE));
	if (!tmp) exit(EXIT_FAILURE);
	tmp->data = data;
	tmp->next = NULL;
	
	// Connecting temp object
	if (top == NULL)
		top = tmp;
	else
	{
		tmp->next = top;
		top = tmp;
	}
}

void StackPop()
{
	if (top == NULL) return;
	NODE* tmp = top->next;
	free(top);
	top = tmp;
}

int StackIsEmpty()
{
	if (top == NULL) return 1;
	else return 0;
}

char StackGet()
{
	return top != NULL ? top->data : 0;
}

void StackDestroy()
{
	while (top != NULL) StackPop();
}
