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
		printf("%d: %d | %s\n", (int)last, (int)last->data, last->data);
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
		free(last->data);
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

NODE* ListAppend(NODE* head, char* data)
{
	NODE* tmp = (NODE*)malloc(sizeof(NODE));
	if (!tmp) exit(EXIT_FAILURE);

	tmp->data = (char*)calloc(strlen(data) + 1, sizeof(char));
	if (!tmp->data) exit(EXIT_FAILURE);

	strcpy_s(tmp->data, strlen(data) + 1, data);
	tmp->next = NULL;

	if (head == NULL) head = tmp;
	else
	{
		NODE* last = ListGetLast(head);
		last->next = tmp;
	}

	return head;
}


void ListCheckEntry(NODE* main, NODE* input)
{

	NODE* main_last = main;
	NODE* input_last = input;

	int input_size = 0;
	while (input_last != NULL)
	{
		input_last = input_last->next;
		input_size++;
	}
	input_last = input;
	int cont = 0;
	int fammiliar = 0;

	while (input_last != NULL)
	{
		main_last = main;
		while (main_last != NULL)
		{
			if (!strcmp(main_last->data, input_last->data))
			{
				cont = 1;

			}

			main_last = main_last->next;
		}


		if (cont) fammiliar++;

		cont = 0;
		input_last = input_last->next;
	}

	if (fammiliar == input_size)
		printf("Main list INCLUDE another one\n");
	else
		printf("Main list DOESN'T INCLUDE another one\n");

}