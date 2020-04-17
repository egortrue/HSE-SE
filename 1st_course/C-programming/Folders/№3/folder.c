#include <io.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "folder.h"

NODE* TreeInit(char* data)
{
	NODE* root = (NODE*)malloc(sizeof(NODE));
	if (!root) exit(EXIT_FAILURE);

	root->type = folder;
	root->data = data;
	root->brother = NULL;
	root->child = NULL;

	return root;
}

NODE* TreeDestroy(NODE* root)
{
	if (root)
	{
		if (root->brother) root->brother = TreeDestroy(root->brother);
		if (root->child) root->child = TreeDestroy(root->child);
		free(root->data);
		free(root);
	}
	return NULL;
}

void BrotherAdd(NODE* old, char* data, int type, int bytes)
{
	NODE* tmp = (NODE*)malloc(sizeof(NODE));
	if (!tmp) exit(EXIT_FAILURE);

	tmp->bytes = bytes;
	tmp->type = type;
	tmp->data = data;
	tmp->brother = NULL;
	tmp->child = NULL;

	NODE* p = old;
	while (p && p->brother)
		p = p->brother;

	if (p) p->brother = tmp;
	else p = tmp;
}

void ChildAdd(NODE* dad, char* data, int type, int bytes)
{
	if (dad->child) BrotherAdd(dad->child, data, type, bytes);
	else
	{
		NODE* tmp = (NODE*)malloc(sizeof(NODE));
		if (!tmp) exit(EXIT_FAILURE);

		tmp->bytes = bytes;
		tmp->type = type;
		tmp->data = data;
		tmp->brother = NULL;
		tmp->child = NULL;

		dad->child = tmp;
	}
}

void TreePrint(NODE* root, int level, FILE* output)
{
	NODE* p = root;
	if (level)
	{
		for (int i = 0; i < level - 1; i++) fprintf(output, "|         ");
		fprintf(output, "*---");
	}

	if (p->type == folder)
		fprintf(output, "[%s]\n", p->data);
	else
		fprintf(output, "%s <%d bytes>\n", p->data, p->bytes);

	if (p->child) TreePrint(p->child, level + 1, output);
	if (p->brother) TreePrint(p->brother, level, output);
}

char* StringCopy(char* str)
{
	char* tmp = (char*)calloc(strlen(str) + 1, sizeof(char));
	if (!tmp) exit(EXIT_FAILURE);
	memcpy_s(tmp, strlen(str) + 1, str, strlen(str));
	return tmp;
}

// All the files in folder
void FolderRun(NODE* root, char* path)
{
	struct _finddata_t f_data;
	intptr_t hFile = _findfirst(path, &f_data);
	if (!hFile)
	{
		_findclose(hFile);
		return;
	}

	do
	{
		// if a loop or parent folder
		if (f_data.name[0] == '.' && f_data.attrib == _A_SUBDIR)
			continue;

		char* data = StringCopy(f_data.name);

		if (f_data.attrib == _A_SUBDIR)
		{
			ChildAdd(root, data, folder, 0);

			// Parent's folder + file name
			char* sub_path = (char*)calloc(PATH_LEN, sizeof(char));
			if (!sub_path) exit(EXIT_FAILURE);
			memset(sub_path, 0, PATH_LEN);
			strcat_s(sub_path, PATH_LEN, path);
			memcpy(sub_path + strlen(sub_path) - 1, f_data.name, sizeof(f_data.name));
			strcat_s(sub_path, PATH_LEN, "\\*");

			NODE* child = root->child;
			while (child->brother) child = child->brother;
			FolderRun(child, sub_path);

		}
		else ChildAdd(root, data, file, (int)f_data.size);


	} while (_findnext(hFile, &f_data) == 0);

	_findclose(hFile);
	return;
}

void FindBigFiles(NODE* root, int max, FILE* output)
{
	if (root)
	{
		if (root->brother) FindBigFiles(root->brother, max, output);
		if (root->child) FindBigFiles(root->child, max, output);
		if (root->type == file)
		{
			if (root->bytes > max)
				fprintf(output, "%s\n", root->data);
		}
	}
}
