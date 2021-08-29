#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "folder.h"

int main()
{
	FILE* output;
	fopen_s(&output, "output.txt", "w");
	if (!output) exit(EXIT_FAILURE);

	//----------------------------------------------

	// "<Local folder>\Folder"
	char* path = (char*)calloc(PATH_LEN, sizeof(char));
	if (!path) exit(EXIT_FAILURE);
	memset(path, 0, PATH_LEN);
	strcat_s(path, PATH_LEN, ".\\Folder");

	char* data1 = StringCopy(path);
	char* data2 = StringCopy(path);
	strcat_s(path, PATH_LEN, "\\*");

	//----------------------------------------------

	printf("Input the file name to search: ");
	char* name = (char*)calloc(100, sizeof(char));
	if (!name) exit(EXIT_FAILURE);
	memset(name, 0, 100);
	gets_s(name, 100);

	//----------------------------------------------

	fprintf(output, "\nFOLDER TREE:\n\n");

	// Init the begin folder
	NODE* root = TreeInit(data1);

	// Create the folder tree
	FolderRun(root, path);
	TreePrint(root, 0, output);
	TreeDestroy(root);

	//----------------------------------------------

	fprintf(output, "\nREZULT FOR %s\n\n", name);

	// Init the begin folder
	root = TreeInit(data2);

	// Create the named folder tree
	FolderRunName(root, path, name);
	TreePrint(root, 0, output);
	TreeDestroy(root);

	//----------------------------------------------

	free(path);
	free(name);
	fclose(output);	
	return 0;
}
