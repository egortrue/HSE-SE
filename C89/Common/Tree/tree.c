#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


NODE* TreeCreate(char* expression)
{
	int expr_len = strlen(expression);
	if (expr_len <= 0) return NULL;

	NODE* root = (NODE*)malloc(sizeof(NODE));
	if (!root) exit(EXIT_FAILURE);

	char symbl = expression[expr_len-1];
	root->symbl = symbl;
	expression[expr_len - 1] = 0;

	// If variable, it is a leaf
	if (96 < symbl && symbl < 123)
	{
		root->type = variable;
		root->left = NULL;
		root->right = NULL;

	}
	else if (symbl == '+' || symbl == '-' || symbl == '*' || symbl == '/')
	{
		root->type  = operation;
		root->left  = TreeCreate(expression);
		root->right = TreeCreate(expression);
	}
	return root;
}

void TreeFillData(NODE* root, int* data)
{
	if (root)
	{
		if (root->type == variable)
		{
			if (data[(int)(root->symbl)-97] != INT_MAX) return;

			int datum;
			printf("Input %c value: ", root->symbl);
			scanf_s("%d", &datum);
			data[(int)(root->symbl)-97] = datum;
			return;
		}

		TreeFillData(root->left, data);
		TreeFillData(root->right, data);
	}
	return;
}

int TreeCalculate(NODE* root, int* data)
{
	if (root)
	{
		if (root->type == variable)
			return data[(int)(root->symbl)-97];
		
		if (root->symbl == '+')
			return TreeCalculate(root->left, data) + TreeCalculate(root->right, data);	
		if (root->symbl == '-')
			return TreeCalculate(root->right, data) - TreeCalculate(root->left, data);
		if (root->symbl == '*')
			return TreeCalculate(root->left, data) * TreeCalculate(root->right, data);
		if (root->symbl == '/') 
			return TreeCalculate(root->right, data) / TreeCalculate(root->left, data);
	}
	return 0;
}

void TreeDestroy(NODE* root)
{
	if (root)
	{
		TreeDestroy(root->left);
		TreeDestroy(root->right);
		free(root);
	}
}

void TreePrint(NODE* root, int level)
{
	if (root)
	{
		TreePrint(root->right, level + 1);

		for (int i = 0; i < level; i++)printf("        ");
		printf("|->(%c:%d)\n", root->symbl, level);

		TreePrint(root->left, level + 1);
	}	
}


//=====================================================================
// Reverse Polish Notation (Sort Station)


typedef struct stack
{
	// last - first empty place
	// if last == 0 --> stack is empty
	int last, SIZE_;
	char* stack;
} STACK;
STACK StackSet(int size)
{
	STACK S;
	S.last = 0;
	S.SIZE_ = size;
	S.stack = (char*)calloc(size, sizeof(char));
	return S;
}
void  StackPrint(STACK* S)
{
	printf("Stack: ");
	for (int i = 0; i < S->last; i++)
			printf("%c ", S->stack[i]);
	puts("");
}
void  StackPush(STACK* S, char ch)
{
	S->stack[S->last++] = ch;
}
char StackPop(STACK* S)
{
	S->last--;
	char ch = S->stack[S->last];
	S->stack[S->last] = 0;
	return ch;
}

char* RPN(char* expression)
{

	char* RPN_str = (char*)calloc(strlen(expression), sizeof(char));
	if (!RPN_str) exit(EXIT_FAILURE);

	STACK S = StackSet(strlen(expression));

	char cur = expression[0];
	int cur_pos = 0, rpn_pos = 0;
	while (cur)
	{

		// Variables
		if (96 < cur && cur < 123)
			RPN_str[rpn_pos++] = cur;

		// Brackets
		if (cur == '(') StackPush(&S, cur);
		if (cur == ')')
		{
			char cur_stack = StackPop(&S);
			while (cur_stack != '(')
			{
				RPN_str[rpn_pos++] = cur_stack;
				if (S.last == 0) break;
				cur_stack = StackPop(&S);
			}
		}

		// Operations
		if (cur == '+' || cur == '-' || cur == '*' || cur == '/')
		{
			if (S.last == 0)
				StackPush(&S, cur);
			else
			{
				char top = StackPop(&S);
				if (top == '(')
				{
					StackPush(&S, top);
					StackPush(&S, cur);
					cur = expression[++cur_pos];
					continue;
				}

				int cur_priority = 0;
				if (cur == '+' || cur == '-') cur_priority = 1;
				if (cur == '*' || cur == '/') cur_priority = 2;


				int top_priority = 0;
				if (top == '+' || top == '-') top_priority = 1;
				if (top == '*' || top == '/') top_priority = 2;

				if (top_priority < cur_priority)
					StackPush(&S, top);

				while (top_priority >= cur_priority)
				{
					RPN_str[rpn_pos++] = top;
					if (S.last == 0) break;
					top = StackPop(&S);
					top_priority = 0;
					if (top == '+' || top == '-') top_priority = 1;
					if (top == '*' || top == '/') top_priority = 2;
				}

				StackPush(&S, cur);
			}
		}
		cur = expression[++cur_pos];
	}

	// Lost operations
	while (S.last)
		RPN_str[rpn_pos++] = StackPop(&S);

	char* rezult = (char*)calloc(rpn_pos+1, sizeof(char));
	if (!rezult) exit(EXIT_FAILURE);

	for (int i = 0; i < rpn_pos; i++)
		rezult[i] = RPN_str[i];

	free(S.stack);
	free(RPN_str);
	return rezult;
}
