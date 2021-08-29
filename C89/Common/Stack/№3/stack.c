#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

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
	NODE* tmp = (NODE*)malloc(sizeof(NODE));
	if (!tmp) exit(EXIT_FAILURE);
	tmp->data = data;
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
	while (top != NULL)
		StackPop();
}


//===============================================================
// LIST AS ADDITION FOR TASK ¹3
// One-linked list
void ListPrint(NODE* head)
{
	printf("List: ");
	NODE* last = head;
	while (last != NULL)
	{
		printf("%c, ", last->data);
		last = last->next;
	}
	printf("\n");
}

void ListDestroy(NODE* head)
{
	NODE* last = head;
	while (last != NULL)
	{
		NODE* tmp = last->next;
		free(last);
		last = tmp;
	}
	head = NULL;
}

NODE* ListGetLast(NODE* head)
{
	NODE* last = head;
	if (last == NULL) return NULL;
	while (last->next) last = last->next;
	return last;
}

NODE* ListAppend(NODE* head, char data)
{
	NODE* tmp = (NODE*)malloc(sizeof(NODE));
	if (!tmp) exit(EXIT_FAILURE);
	tmp->data = data;
	tmp->next = NULL;

	if (head == NULL) head = tmp;
	else
	{
		NODE* last = ListGetLast(head);
		last->next = tmp;
	}

	return head;
}

char ListGetData(NODE* head, int level)
{
	NODE* last = head;
	for (int i = 0; i < level; i++)
		if (last != NULL)
			last = last->next;

	if (last != NULL)
		return last->data;
}
