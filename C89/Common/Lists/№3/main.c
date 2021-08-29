#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{

	char* examples[] = { "121123", "312312312", "3123123121", "-3137542",
					   "12.3", "-3.1415",  "-999999999999999",
					   "asdf 123 asdf", "aADASadgs1dfgDWfasdfasdf11asdfas",
					   "aADASadgs1dfgDWQETsdf11sdfas241241",
					   "as1dfgDWQETWEf123ffasdf11a241", "1241231231.1231",
					   "sdfasdfas241",  "123aADASadgs1dETWEf12fasdf11asd",
					   "11.11",  "f11gjb345241241" };

	NODE* L1 = NULL;
	NODE* L2 = NULL;

	srand(time(0));
	for (int i = 0; i < 1; i++) L1 = ListAppend(L1, examples[rand() % 16]);
	for (int i = 0; i < 3; i++) L2 = ListAppend(L2, examples[rand() % 16]);

	ListPrint(L1);
	ListPrint(L2);
	ListCheckEntry(L2, L1);

	ListDestroy(L1);
	ListDestroy(L2);
	return 0;
}

