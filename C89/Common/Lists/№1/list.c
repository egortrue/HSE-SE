#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// One-linked list
typedef struct node
{
	int data;
	struct node* next;
}NODE;

NODE* head = NULL;

void ListPrint()
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

void ListDestroy()
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

NODE* ListGetLast()
{
	NODE* last = head;
    if (last == NULL) return NULL;
    while (last->next) last = last->next;
    return last;
}

void ListAppend(int data)
{
	NODE* tmp = (NODE*)malloc(sizeof(NODE));
	if (!tmp) exit(EXIT_FAILURE);
	tmp->data = data;
	tmp->next = NULL;


	NODE* last = ListGetLast();
	if (last == NULL) head = tmp;
	else last->next = tmp;
}

void ListCheckSameData()
{
	int buffer[1000];
	int i = 0;
	for (i = 0; i < 1000; i++) buffer[i] = INT_MAX;
	i = 0;


	NODE* last = head;
	while (last != NULL)
	{
		for (int j = 0; j < i; j++)
			if (buffer[j] == last->data)
			{
				printf("There are EXIST the same data: %d", last->data);
				return;
			}
		buffer[i++] = last->data;
		last = last->next;
	}
	printf("There are NOT EXIST the same data");
}
