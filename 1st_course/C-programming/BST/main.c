
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "bst.h"

int main()
{
	
	srand(time(0));
	NODE* root = TreeCreate(100);
	for (int i = 0; i < 20; i++)
		root = TreePut(root, rand() % 200);
	TreePrint(root, 0);
	TreeDestroy(root);

	return 0;
}
