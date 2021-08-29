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

	// Print in reverse order
	while (!StackIsEmpty())
	{
		printf("%c", StackGet());
		StackPop();
	}

	// Free the stack
	StackDestroy();
	return 0;
}
