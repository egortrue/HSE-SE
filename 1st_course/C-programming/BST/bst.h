#ifndef BST_H
#define BST_H

typedef struct node
{
	int data;
	struct node* left;
	struct node* right;
}NODE;

NODE* TreeCreate  (int data);
NODE* TreePut     (NODE* root, int data);
void  TreeDestroy (NODE* root);
void  TreePrint   (NODE* root, int level);

#endif
