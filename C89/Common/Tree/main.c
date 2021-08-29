#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

#define EXPR_LEN_MAX 200


int main()
{
	char* buffer = (char*)calloc(EXPR_LEN_MAX, sizeof(char));
	if (!buffer) exit(EXIT_FAILURE);

	// Init the expression
	printf("Input expression: ");
	gets_s(buffer, EXPR_LEN_MAX);
	int expr_len = strlen(buffer);

	char* expression = (char*)calloc(expr_len + 1, sizeof(char)); // +1 for zero
	if (!expression) exit(EXIT_FAILURE);
	for (int i = 0; i < expr_len; i++)
		expression[i] = buffer[i];
	free(buffer); // buffer worked out

	// Init the RPN
	char* rpn = RPN(expression);
	free(expression); // expression worked out
	printf("Reverse Polish Notation: %s\n", rpn);

	// Init the tree
	NODE* tree = TreeCreate(rpn);
	printf("Tree:\n\n");
	TreePrint(tree, 0);
	printf("\n\n");
	free(rpn); // rpn worked out

	// Init the variables
	int* data = (int*)calloc(26, sizeof(int));
	if (!data) exit(EXIT_FAILURE);
	for (int i = 0; i < 26; i++) data[i] = INT_MAX;
	TreeFillData(tree, data);

	// Rezult
	int answer = TreeCalculate(tree, data);
	printf("\nThe answer: %d\n", answer);
	
	TreeDestroy(tree);
	free(data);
}
