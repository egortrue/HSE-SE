#include "stack.h"
#include <stdio.h>

int main()
{
	char brackets[] = "[][[{()}]][][{}][]([])(())()(){}{}{}{}";

	// Push to the stack
	int i = 0;
	while (brackets[i])
		StackPush(brackets[i++]);

	printf("Orig: %s\n", brackets);
	printf("Stack: ");
	StackPrint();

	// This is like a stack but it is common array ><
	// Closed brackets are contained here
	char buffer[100] = { 0 }; 
	int buf_last = 0;
	while (!StackIsEmpty())
	{
		char symbl = StackGet();
		StackPop();

		if (buf_last == 0 &&  (symbl == '(' ||
			                   symbl == '[' ||
			                   symbl == '{'))
		{
			printf("\n%c is not closed\n", symbl);
			break;
		}

		if (symbl == ')' || symbl == ']' || symbl == '}')
			buffer[buf_last++] = symbl;

		if (buf_last > 0)
			if ((symbl == '(' && buffer[buf_last-1] == ')') ||
				(symbl == '[' && buffer[buf_last-1] == ']') ||
				(symbl == '{' && buffer[buf_last-1] == '}'))
				buffer[buf_last--] = 0;

	}

	if (StackIsEmpty() && buf_last == 0)
		printf("This is right brackets expression\n");
	else
		printf("This is wrong brackets expression\n");

	// Free the stack
	StackDestroy();
	return 0;
}
