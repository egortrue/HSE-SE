#include "list.h"
#include <stdlib.h>
#include <time.h>


int main()
{

	NODE* L1 = NULL;
	NODE* L2 = NULL;
	NODE* L3 = NULL;

	srand(time(0));
	for (int i = 0; i < 5; i++) L1 = ListAppend(L1, rand() % 10);
	for (int i = 0; i < 5; i++) L2 = ListAppend(L2, rand() % 10);
	L3 = ListCreateDiff(L1, L2);

	ListPrint(L1);
	ListPrint(L2);
	ListPrint(L3);

	ListDestroy(L1);
	ListDestroy(L2);
	ListDestroy(L3);


	return 0;
}
