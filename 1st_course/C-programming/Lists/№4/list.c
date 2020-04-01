#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



void ListPrint(NODE* head)
{
	printf("    List:\n");
	NODE* last = head;
	while (last != NULL)
	{
		printf("%d: %c\n", (int)last, last->data);
		last = last->next;
	}
	printf("=======================\n");
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

NODE* ListAppend(NODE* head, unsigned char data)
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


void ListSeparateNumsAndLetters(NODE* list, NODE** nums, NODE** letters)
{
	NODE* list_last = list;
	while (list_last != NULL)
	{
		if (47 < list_last->data && list_last->data < 58)
			*nums = ListAppend(*nums, list_last->data);

		if (64 < list_last->data && list_last->data < 91)
			*letters = ListAppend(*letters, list_last->data);

		if (96 < list_last->data && list_last->data < 123)
			*letters = ListAppend(*letters, list_last->data);

		list_last = list_last->next;
	}
}