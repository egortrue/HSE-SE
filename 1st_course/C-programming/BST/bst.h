#ifndef BST_H
#define BST_H

typedef struct node
{
	int data;
	struct node* left;
	struct node* right;
}NODE;

NODE* TreePut     (NODE* root, int data);
void  TreeDestroy (NODE* root);
void  TreePrint   (NODE* root, int level, FILE* output);
void  TreeSize    (NODE* root, int* count);

// Task 1:
void FindMaxFullTree (NODE* root, NODE** max_tree, int* max_size);

// Task 2:
void FindLongestLinearTree(NODE* root, NODE** long_tree, int* size);


#endif
