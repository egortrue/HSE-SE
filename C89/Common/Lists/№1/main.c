#include "list.h"
#include <stdlib.h>
#include <time.h>

int main()
{
	srand(time(0));
	for (int i = 0; i < 10; i++)
		ListAppend(rand() % 20);
	ListPrint();
	ListCheckSameData();
	ListDestroy();
	return 0;
}

