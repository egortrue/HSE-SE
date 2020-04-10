#ifndef TREE_H
#define TREE_H

typedef struct node
{
	enum 
	{ 
		variable, // a, b, ... z 
		operation // +, -, *, /
	} type;

	char symbl;
	struct node* left;
	struct node* right;
}NODE;

NODE* TreeCreate(char* expression);
void  TreeFillData(NODE* root, int* data);
int   TreeCalculate(NODE* root, int* data);

void  TreeDestroy(NODE* root);
void  TreePrint(NODE* root, int level);

char* RPN(char* expression);

#endif
