#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{

	NODE* LIST = NULL;
	NODE* NUMS = NULL;
	NODE* LETTERS = NULL;

	srand(time(0));
	for (int i = 0; i < 20; i++) LIST = ListAppend(LIST, rand() % 255);
	ListSeparateNumsAndLetters(LIST, &NUMS, &LETTERS);

	ListPrint(LIST);
	ListPrint(NUMS);
	ListPrint(LETTERS);

	ListDestroy(LIST);
	ListDestroy(NUMS);
	ListDestroy(LETTERS);
	return 0;
}

