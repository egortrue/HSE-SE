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

	char* data = StringCopy(path);
	strcat_s(path, PATH_LEN, "\\*");

	//----------------------------------------------

	fprintf(output, "\nFOLDER TREE:\n\n");

	// Init the begin folder
	NODE* root = TreeInit(data);

	// Create the folder tree
	FolderRun(root, path);
	TreePrint(root, 0, output);

	//----------------------------------------------

	int count = 0;
	FileCount(root, &count);
	fprintf(output, "\n\nFILES IN THIS FOLDER: %d\n", count);

	//----------------------------------------------

	TreeDestroy(root);
	free(path);
	fclose(output);
	return 0;
}
