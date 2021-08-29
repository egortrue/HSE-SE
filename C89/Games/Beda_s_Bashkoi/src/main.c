
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>

#include "dict.h"
#include "interface.h"

// Beda S Bashkoi

// max len of name = 28
// max len of val = 52
// max len of key = 10
// 18 rows on 1 page

int main()
{

	ConsoleRemake();

	Intro();

	// Title
	ScreenSetColor(Black, White);
	ScreenFill(0, 101, 2, 36, 3);
	CursorSetPosition(112, 2);
	printf("BEDA S BASHKOI");
	CursorSetPosition(107, 3);
	printf("Game of the Year Edition");
	ScreenFill('=', 104, 4, 30, 0);

	// Menu frame
	ScreenSetColor(Cyan, White);
	ScreenFill(223, 100, 1, 38, 1);
	ScreenSetColor(Cyan, Black);
	ScreenFill(220, 100, 38, 38, 1);
	ScreenFill(219, 100, 1, 1, 38);
	ScreenFill(219, 137, 1, 1, 38);
	ScreenFill(223, 101, 5, 36, 1);
	ScreenSetColor(LightGray, Black);


	DICT** bookshelf = (DICT**)calloc(14, sizeof(DICT*));
	if (!bookshelf) exit(EXIT_FAILURE);

	DICT* example = DictCreate("Example of Dumb Dict");
	{
		char* values[] = { "121123", "312312312", "3123123121", "-3137542",
						   "12.3", "-3.1415",  "-999999999999999",
						   "asdf 123 asdf", "aADASadgs1dfgDWfasdfasdf11asdfas",
						   "aADASadgs1dfgDWQETsdf11sdfas241241",
						   "as1dfgDWQETWEf123ffasdf11a241", "1241231231.1231",
						   "sdfasdfas241",  "123aADASadgs1dETWEf12fasdf11asd",
						   "11.11",  "f11gjb345241241" };

		char* key = (char*)malloc(sizeof(char) * 11);
		if (!key) exit(EXIT_FAILURE);
		strcpy_s(key, 8, "example");

		srand((int)time(0));
		for (int i = 0; i < 1000; i++)
		{
			memset(&key[7], (i / 100) + 48, 1);
			memset(&key[8], (i % 100) / 10 + 48, 1);
			memset(&key[9], (i % 10) + 48, 1);
			memset(&key[10], 0, 1);
			DictItemPut(example, key, values[rand() % 16]);
		}
		free(key);
	}
	DICT* empty = DictCreate("Emtpy Dictionary");

	bookshelf[0] = example;
	bookshelf[1] = empty;

	BookShelf(bookshelf);

	for (int i = 0; i < 10; i++)
		if (bookshelf[i] != NULL)
			DictDestroy(bookshelf[i]);
	free(bookshelf);

	system("cls");
	CursorSetPosition(0, 36);  // 4 rows for default debug output info
	return 0;
}