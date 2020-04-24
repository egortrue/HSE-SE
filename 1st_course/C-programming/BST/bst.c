#include <stdlib.h>
#include <stdio.h>

#include "bst.h"

NODE* TreeCreate(int data)
{
	NODE* root = (NODE*)malloc(sizeof(NODE));
	if (!root) exit(EXIT_FAILURE);

	root->data  = data;
	root->left  = NULL;
	root->right = NULL;

	return root;
}

NODE* TreePut(NODE* root, int data)
{
	if (root)
	{
		if (root->data < data)
			root->right = TreePut(root->right, data);
		else
			root->left = TreePut(root->left, data);
		return root;
	}

	NODE* tmp = (NODE*)malloc(sizeof(NODE));
	if (!tmp) exit(EXIT_FAILURE);

	tmp->data = data;
	tmp->left = NULL;
	tmp->right = NULL;

	return tmp;
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

		for (int i = 0; i < level; i++)printf("|    ");
		printf("|==(%d)\n", root->data);

		TreePrint(root->left, level + 1);
	}
}
