#include "stack.h"
#include <stdio.h>

int main()
{

	// Create the list
	NODE* origList = NULL;
	for (int i = 0; i < 10; i++) origList = ListAppend(origList, i + 64);

	ListPrint(origList); 

	// Push in the stack
	for (int i = 0; i < 10; i++)
		StackPush(ListGetData(origList, i));

	// Create new list as copy
	NODE* copyList = NULL;
	for (int i = 0; i < 10; i++)
	{
		copyList = ListAppend(copyList, StackGet());
		StackPop();
	}

	// Print new reversed list
	ListPrint(copyList); 

	// Free memory
	ListDestroy(origList);
	StackDestroy();
	ListDestroy(copyList);
	return 0;
}
