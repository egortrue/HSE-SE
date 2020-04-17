#ifndef FOLDER_H
#define FOLDER_H

#define PATH_LEN 1000

typedef struct node
{
	enum {file, folder} type;
	char* data;
	struct node* child;
	struct node* brother;
}NODE;

NODE* TreeInit    (char* data);
NODE* TreeDestroy (NODE* root);

char* StringCopy  (char* str);
void  TreePrint   (NODE* root, int level, FILE* output);
void  FolderRun   (NODE* root, char* path);

// Task 1:
int FolderRunName(NODE* root, char* path, char* name);

#endif
