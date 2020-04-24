#include <stdlib.h>
#include <stdio.h>

#include "bst.h"

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

void TreePrint(NODE* root, int level, FILE* output)
{
	if (root)
	{
		TreePrint(root->right, level + 1, output);

		for (int i = 0; i < level; i++) fprintf(output, "|      ");
		fprintf(output, "|--(%3d)\n", root->data);

		TreePrint(root->left, level + 1, output);
	}
}

void TreeSize(NODE* root, int* count)
{
	if (root)
	{
		if (root->left) TreeSize(root->left, count);
		if (root->right) TreeSize(root->right, count);
		(*count)++;
	}
}

//=============================================================
// Task 1:
NODE* FullTree(NODE* root, NODE* full)
{
	if (root) 
	{
		full = TreePut(full, root->data);
		if (root->left && root->right)
		{ 
			FullTree(root->right, full);
			FullTree(root->left, full);
		}
	}
	return full;
}


void FindMaxFullTree(NODE* root, NODE** max_tree, int* max_size)
{
	if (root)
	{
		NODE* tmp = FullTree(root, NULL);
		int size = 0;
		TreeSize(tmp, &size);

		if (*max_size < size)
		{
			TreeDestroy(*max_tree);
			*max_size = size;
			*max_tree = tmp;
		}
		else
		{
			TreeDestroy(tmp);
		}

		FindMaxFullTree(root->left, max_tree, max_size);
		FindMaxFullTree(root->right, max_tree, max_size);
	}
}

//=============================================================
// Task 2

NODE* LinearTree(NODE* root, NODE* line, int left)
{
	if (root)
	{
		line = TreePut(line, root->data);
		if (left && root->left && !root->right)
			LinearTree(root->left, line, 1);
		else if (!left && root->right && !root->left)
			LinearTree(root->left, line, 0);
	}
	return line;
}

void FindLongestLinearTree(NODE* root, NODE** long_tree, int* max_size)
{
	if (root)
	{
		NODE* left = LinearTree(root, NULL, 1);
		NODE* right = LinearTree(root, NULL, 0);

		int size_left = 0, size_right = 0;
		TreeSize(left, &size_left);
		TreeSize(right, &size_right);

		if ((size_left < *max_size) && (size_right < *max_size))
		{
			TreeDestroy(left);
			TreeDestroy(right);
		}
		else
		{
			TreeDestroy(*long_tree);
			if (size_left > *max_size)
			{
				TreeDestroy(right);	
				*max_size = size_left;
				*long_tree = left;
			}
			else
			{
				TreeDestroy(left);
				*max_size = size_right;
				*long_tree = right;
			}
		}

		FindLongestLinearTree(root->left, long_tree, max_size);
		FindLongestLinearTree(root->right, long_tree, max_size);
	}
}

