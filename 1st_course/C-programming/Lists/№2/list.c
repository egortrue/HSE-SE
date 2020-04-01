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
		printf("%d: %d\n", (int)last, last->data);
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

NODE* ListAppend(NODE* head, int data)
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

NODE* ListCreateDiff(NODE* L1, NODE* L2)
{
	NODE* L3 = NULL;

	int buffer[1000] = { 0 };
	int i = 0;
	for (i = 0; i < 1000; i++) buffer[i] = INT_MAX;
	i = 0;

	int skip = 0;
	NODE* L1_last = L1;
	NODE* L2_last = L2;

	while (L1_last != NULL)
	{
		while (L2_last != NULL)
		{
			if (L1_last->data == L2_last->data) skip = 1;
			L2_last = L2_last->next;
		}

		if (skip)
		{
			skip = 0;
			L1_last = L1_last->next;
			continue;
		}

		L3 = ListAppend(L3, L1_last->data);
		L1_last = L1_last->next;
	}
	return L3;
}