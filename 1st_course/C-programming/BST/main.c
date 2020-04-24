
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "bst.h"

int main()
{ 
	FILE* output;
	fopen_s(&output, "output.txt", "w");
	if (!output) exit(EXIT_FAILURE);

	//========================================================

	fprintf(output, "Random Tree:\n");
	srand(time(0));
	NODE* root = TreePut(NULL, 100);
	for (int i = 0; i < 20; i++)
		TreePut(root, rand() % 200);

	int size = 0;
	TreeSize(root, &size);
	fprintf(output, "size = %d\n\n", size);
	TreePrint(root, 0, output);

	//========================================================
	// Task 1

	fprintf(output, "\n=====================================================\n");
	fprintf(output, "Max full tree:\n");

	int full_tree_size = 0;
	NODE* full_tree = NULL;
	FindMaxFullTree(root, &full_tree, &full_tree_size);

	fprintf(output, "size = %d\n\n", full_tree_size);
	TreePrint(full_tree, 0, output);
	TreeDestroy(full_tree);

	//========================================================
	// Task 2

	fprintf(output, "\n=====================================================\n");
	fprintf(output, "The longest linear tree:\n");

	int long_tree_size = 0;
	NODE* long_tree = NULL;
	FindLongestLinearTree(root, &long_tree, &long_tree_size);

	fprintf(output, "size = %d\n\n", long_tree_size);
	TreePrint(long_tree, 0, output);
	TreeDestroy(long_tree);

	TreeDestroy(root);
	return 0;
}
