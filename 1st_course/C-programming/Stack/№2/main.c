#include "stack.h"
#include <stdio.h>

int main()
{
	// Read from a keyboard
	char string[1000] = { 0 };
	fgets(string, 1000, stdin);

	// Push in the stack
	int i = 0;
	while (string[i]) StackPush(string[i++]);
	StackPop(); // Remove last "Enter" symbol

	i = 0;
	while (!StackIsEmpty())
	{
		char symb = StackGet();
		StackPop();
		if (symb == string[i++]) continue;
		else break;	
	}

	if (StackIsEmpty()) printf("This IS a polindrom");
	else printf("This IS NOT a polindrom");

	// Free the stack
	StackDestroy();
	return 0;
}
