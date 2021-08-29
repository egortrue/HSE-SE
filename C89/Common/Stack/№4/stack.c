#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node  // 4 bytes + 4 bytes => 8 bytes
{
	struct node* next;
	char* data;
}NODE;

NODE* top = NULL;

void StackPrint()
{
	NODE* tmp = top;
	while (tmp != NULL)
	{
		printf("-> %s\n", tmp->data);
		tmp = tmp->next;
	}
	printf("\n");
}

void StackPush(char* data)
{
	NODE* tmp = (NODE*)malloc(sizeof(NODE));
	if (!tmp) exit(EXIT_FAILURE);

	tmp->data = (char*)calloc(strlen(data)+1, sizeof(char)); // strlen() + 1 for zero on the end of string
	if (!tmp->data) exit(EXIT_FAILURE);

	memcpy(tmp->data, data, strlen(data));

	tmp->next = NULL;

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
	free(top->data);
	free(top);
	top = tmp;
}

int StackIsEmpty()
{
	if (top == NULL) return 1;
	else return 0;
}

char* StackGet()
{
	return top == NULL ? 0 : top->data;
}

void StackDestroy()
{
	while (top != NULL) StackPop();
}
