#include "interface.h"
#include "dict.h"

#include <windows.h>
#include <stdio.h>
#include <conio.h>


void CursorVisizible(int n)
{
	// --------- Invisizible cursor ---------
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = n;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	// --------------------------------------
}

void ConsoleRemake()
{
	// I don't know what exactly there happen xD
	// (PLEASE DO NOT CHANGE IT)

	// Set console box size (140x40)
	system("mode con cols=140 lines=40");

	// Make console box unsizable
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// Full screen
	//SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);

	// ------- Hide the scroll bar -------
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	csbi.dwSize.X = csbi.dwMaximumWindowSize.X;
	csbi.dwSize.Y = csbi.dwMaximumWindowSize.Y;
	SetConsoleScreenBufferSize(hstdout, csbi.dwSize);

	HWND x = GetConsoleWindow();
	ShowScrollBar(x, SB_BOTH, FALSE);
	// ------------------------------------

	CursorVisizible(FALSE);

}

void CursorSetPosition(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ScreenSetColor(int text, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((bg << 4) | text));
}


void Toy()
{
	int x = 102, y = 26;
	CursorSetPosition(x, y);    printf("          .--.   _               ");
	CursorSetPosition(x, y+1);  printf("      .---|__| .((\\=.             ");
	CursorSetPosition(x, y+2);  printf("   .--|===|--|/    ,(,     Yes,  ");
	CursorSetPosition(x, y+3);  printf("   |  |===|  |\\      y   This is ");
	CursorSetPosition(x, y+4);  printf("   |  |   |  | `.__,'       a bear");
	CursorSetPosition(x, y+5);  printf("   |  |   |  | /  \\\\\\             ");
	CursorSetPosition(x, y+6);  printf("   |  |   |  |/|  | \\`----.       ");
	CursorSetPosition(x, y+7);  printf("   |  |   |  ||\\  \\  |___.'_      ");
	CursorSetPosition(x, y+8);  printf("  _|  |   |__||,\\  \\-+-._.' )_    ");
	CursorSetPosition(x, y+9);  printf(" / |  |===|--|\\  \\  \\      /  \\   ");
	CursorSetPosition(x, y+10); printf("/  `--^---'--' `--`-'---^-'    \\  ");
	CursorSetPosition(x, y+11); printf("'================================`");
}


void Book(int x, int y)
{
	CursorSetPosition(x, y);    printf("   ,   ,");
	CursorSetPosition(x, y+1);  printf("  /////|");
	CursorSetPosition(x, y+2);  printf(" ///// |");
	CursorSetPosition(x, y+3);  printf("|~~~|  |");
	CursorSetPosition(x, y+4);  printf("|===|  |");
	CursorSetPosition(x, y+5);  printf("|   |  |");
	CursorSetPosition(x, y+6);  printf("|   |  |");
	CursorSetPosition(x, y+7);  printf("|   | / ");
	CursorSetPosition(x, y+8);  printf("|===|/  ");
	CursorSetPosition(x, y+9);  printf("'---'   ");
}

void BookShelf(DICT** bookshelf)
{
	Toy();
	ScreenSetColor(Cyan, Black);
	ScreenFill(223, 101, 11, 36, 1);
	ScreenSetColor(LightGray, Black);

	CursorSetPosition(48, 2);
	printf("BOOKSHELF");
	for (int i = 0; i < 3; i++)
	{
		ScreenFill(223, 2, 5 + (i * 14), 95, 1);  // top horizontal
		ScreenFill(220, 2, 6 + (i * 14), 95, 1);  // bot horizontal
		ScreenFill(219, 2, 5 + (i * 14), 1, 2);  // 1 vertical
		ScreenFill(219, 97, 5 + (i * 14), 1, 2);  // 2 vertical
	}

	ActionClear();
	CursorSetPosition(103, 12);
	printf("ENTER - Choose a book");
	CursorSetPosition(103, 13);
	printf("DELETE - Erase a book");
	CursorSetPosition(103, 14);
	printf("ESCAPE - Exit the game");

	int choose = 0;
	int action = 0;
	do
	{
		// Books
		for (int i = 0; i < 14; i++)
		{
			if (i == choose)
			{
				if (bookshelf[choose] != NULL) ScreenSetColor(LightGreen, Black);
				else ScreenSetColor(LightRed, Black);
			}
			else ScreenSetColor(LightGray, Black);
			if (i < 7)
				Book(4 + (14 * i), 8);
			else
				Book(4 + (14 * (i % 7)), 8 + 14);
		}
		ScreenSetColor(LightGray, Black);

		// Book's info
		InfoClear();
		if (bookshelf[choose] != NULL)
			DictInfo(bookshelf[choose], bookshelf[choose]->actual_size / 18 + 1);
		else
		{
			CursorSetPosition(109, 7);
			printf("This is empty book");
			CursorSetPosition(103, 8);
			printf("Press ENTER to create Dictionary");
		}


		// Actions
		ScreenSetColor(LightGray, Black);
		BUTTON button = _getch();
		if (button == ESC)
			return;
		else if (button == DOWN)
		{
			if (choose > 6) continue;
			choose += 7;
		}
		else if (button == UP)
		{
			if (choose < 7) continue;
			choose -= 7;
		}
		else if (button == LEFT)
		{
			if (choose == 0 || choose == 7) continue;
			choose--;
		}
		else if (button == RIGHT)
		{
			if (choose == 6 || choose == 13) continue;
			choose++;
		}
		else if (button == DEL)
		{
			if (bookshelf[choose] != NULL)
			{
				DictDestroy(bookshelf[choose]);
				bookshelf[choose] = NULL;
			}
		}
		else if (button == ENTER)
		{
			if (bookshelf[choose] != NULL)
			{
				InfoClear();
				ActionClear();
				TableClear();
				Dictionary(bookshelf[choose]);
				TableClear();
				CursorSetPosition(48, 2);
				printf("BOOKSHELF");
				for (int i = 0; i < 3; i++)
				{
					ScreenFill(223, 2, 5 + (i * 14), 95, 1);  // top horizontal
					ScreenFill(220, 2, 6 + (i * 14), 95, 1);  // bot horizontal
					ScreenFill(219, 2, 5 + (i * 14), 1, 2);  // 1 vertical
					ScreenFill(219, 97, 5 + (i * 14), 1, 2);  // 2 vertical
				}
				ActionClear();
				CursorSetPosition(103, 12);
				printf("ENTER - Choose a book");
				CursorSetPosition(103, 13);
				printf("DELETE - Erase a book");
				CursorSetPosition(103, 14);
				printf("ESCAPE - Exit the game");
			}
			else
			{
				InfoClear();
				CursorSetPosition(109, 6);
				printf("Create a Dictionary");
				CursorSetPosition(103, 8);
				printf("Name: ");
				CursorVisizible(TRUE);
				// name init from stdin
				int i = 0;
				char name_[25];
				memset(name_, 0, 25);
				CursorSetPosition(109, 8);
				while (1)
				{
					name_[i] = _getch();
					if (name_[i] == ENTER)
						break;
					if (i == 25) continue;
					printf("%c", name_[i++]);
				}
				char* name = (char*)calloc(i + 1, sizeof(char));
				if (!name) exit(EXIT_FAILURE);

				memmove(name, name_, i);
				memset(&name[i], 0, 1);
				bookshelf[choose] = DictCreate(name);
				free(name);
				CursorVisizible(FALSE);
			}
				
		}

	} while (1);
}

void TextPrint(int text, int bg, char* str)
{
	ScreenSetColor(text, bg);
	printf("%s", str);
	ScreenSetColor(LightGray, Black);
}

void ScreenFill(int ch, int x, int y, int xl, int yl)
{
	CursorSetPosition(x, y);
	for (int i = 0; i <= yl; i++)
	{
		for (int j = 0; j < xl; j++)
			printf("%c", ch);
		CursorSetPosition(x, y + i);
	}
	CursorSetPosition(x, y);
}

// Often used types of ScreenFill()
void TableClear() { ScreenFill(0, 0, 0, 100, 40); }
void InfoClear() { ScreenFill(0, 101, 6, 36, 5); }
void ActionClear() { ScreenFill(0, 101, 12, 36, 14); }


void DictInfo(DICT* d, int page)
{
	InfoClear();
	CursorSetPosition(103, 6);
	printf("Dict: %-20s", d->name);
	CursorSetPosition(103, 7);
	printf("Actual size: %d words", d->actual_size);
	CursorSetPosition(103, 8);
	printf("Maximal size: %d words", d->max_size);
	CursorSetPosition(103, 9);
	printf("Pages: %d/%d", page, d->actual_size / 18 + 1);
}

void DictPrint(DICT* d, int page)
{
	TableClear();
	ScreenSetColor(LightGray, Black);

	int i = (page - 1) * 18, c = 0;
	for (i; i < d->actual_size; i++, c += 2)
	{
		// Print key
		if (i % 18 == 0 && c != 0) break;
		CursorSetPosition(3, 2 + c);
		printf(" key: %-10s   type: ", d->items[i]->key);

		// Print type and value
		if (d->items[i]->type_val == STRING)
		{
			TextPrint(LightGreen, Black, "string  ");
			printf("  value: %-52s", d->items[i]->str);
		}

		if (d->items[i]->type_val == INTEGER)
		{
			TextPrint(LightBlue, Black, "integer ");
			printf("  value: %-52d", d->items[i]->num);
		}

		if (d->items[i]->type_val == _DOUBLE_)
		{
			TextPrint(LightRed, Black, "double  ");
			printf("  value: %-52lf", d->items[i]->f_num);
		}

		// Print separators
		CursorSetPosition(3, 3 + c);
		ScreenSetColor(DarkGray, Black);
		for (int j = 0; j <= 93; j++) printf("-");
		ScreenSetColor(LightGray, Black);

	}

	// Table's frame
	ScreenFill(223, 2, 1, 95, 1); // top horizontal
	ScreenFill(220, 2, c + 2, 95, 1); // bot horizontal
	ScreenFill(219, 2, 1, 1, c + 2); // 1 vertical
	ScreenFill(219, 20, 1, 1, c + 2); // 2 vertical
	ScreenFill(219, 36, 1, 1, c + 2); // 3 vertical
	ScreenFill(219, 97, 1, 1, c + 2); // 4 vertical
}

void Dictionary(DICT* d)
{

	ScreenSetColor(Cyan, Black);
	ScreenFill(223, 101, 11, 36, 1);
	ScreenSetColor(LightGray, Black);
	int action = 0;
	int page = 1;
	DictPrint(d, page);
	DictInfo(d, page);
	do
	{
		// Actions
		CursorSetPosition(103, 12);
		if (action == 0) TextPrint(Black, White, "0) Bookshelf");
		else printf("0) Bookshelf");

		CursorSetPosition(103, 14);
		if (action == 1) TextPrint(Black, White, "1) Previous page");
		else printf("1) Previous page");
		CursorSetPosition(103, 15);
		if (action == 2) TextPrint(Black, White, "2) Next page");
		else printf("2) Next page");
		CursorSetPosition(103, 16);
		if (action == 3) TextPrint(Black, White, "3) First page");
		else printf("3) First page");
		CursorSetPosition(103, 17);
		if (action == 4) TextPrint(Black, White, "4) Last page");
		else printf("4) Last page");

		CursorSetPosition(103, 19);
		if (action == 5) TextPrint(Black, White, "5) Append item");
		else printf("5) Append item");
		CursorSetPosition(103, 20);
		if (action == 6) TextPrint(Black, White, "6) Remove item");
		else printf("6) Remove item");
		CursorSetPosition(103, 21);
		if (action == 7) TextPrint(Black, White, "7) Get item");
		else printf("7) Get item");

		CursorSetPosition(103, 23);
		if (action == 8) TextPrint(Black, White, "8) Get keys");
		else printf("8) Get keys");
		CursorSetPosition(103, 24);
		if (action == 9) TextPrint(Black, White, "9) Get values");
		else printf("9) Get values");



		BUTTON button = _getch();
		char key_[11];
		char* key;	
		char value_[25];
		char* value;
		int i = 0;
		int c = 0;

		if (button == DOWN)
		{
			if (action == 9) continue;
			action++;
		}
		else if (button == UP)
		{
			if (action == 0) continue;
			action--;
		}
		else if (button == ENTER)
		{
			if (action == 0)
				break;
			switch (action)
			{
			case 1:
				if (page == 1) continue;
				page--;
				break;
			case 2:
				if (page == (d->actual_size) / 18 + 1) continue;
				page++;
				break;
			case 3:
				if (page == 1) continue;
				page = 1;
				break;
			case 4:
				if (page == (d->actual_size) / 18 + 1) continue;
				page = (d->actual_size) / 18 + 1;
				break;
			case 5:
				ScreenFill(0, 101, 6, 36, 5);
				CursorSetPosition(106, 6);
				printf("Append Item to Dictionary");
				CursorSetPosition(103, 8);
				printf("Key: ");
				CursorSetPosition(103, 9);
				printf("Value: ");

				CursorVisizible(TRUE);
				i = 0;
				// key init from stdin
				memset(key_, 0, 11);
				CursorSetPosition(108, 8);
				while (1)
				{
					key_[i] = _getch();
					if (key_[i] == ENTER)
						break;
					if (i == 10) continue;
					printf("%c", key_[i++]);
				}
				key = (char*)calloc(i + 1, sizeof(char));
				if (!key) exit(EXIT_FAILURE);
				memmove(key, key_, i);
				memset(&key[i], 0, 1);

				i = 0;
				// value init from stdin
				memset(value_, 0, 25);
				CursorSetPosition(110, 9);
				while (1)
				{
					value_[i] = _getch();
					if (value_[i] == ENTER)
						break;
					if (i == 25) continue;
					printf("%c", value_[i++]);
				}
				value = (char*)calloc(i + 1, sizeof(char));
				if (!value) exit(EXIT_FAILURE);
				memmove(value, value_, i);
				memset(&value[i], 0, 1);

				// append item
				CursorVisizible(FALSE);
				DictItemPut(d, key, value);
				free(key);
				free(value);
				page = (d->actual_size) / 18 + 1;
				break;
			case 6:
				ScreenFill(0, 101, 6, 36, 5);
				CursorSetPosition(105, 6);
				printf("Delete Item from Dictionary");
				CursorSetPosition(103, 8);
				printf("Key: ");

				CursorVisizible(TRUE);
				i = 0;
				// key init from stdin
				memset(key_, 0, 11);
				CursorSetPosition(108, 8);
				while (1)
				{
					key_[i] = _getch();
					if (key_[i] == ENTER)
						break;
					if (i == 10) continue;
					printf("%c", key_[i++]);
				}
				key = (char*)calloc(i + 1, sizeof(char));
				if (!key) exit(EXIT_FAILURE);
				memmove(key, key_, i);
				memset(&key[i], 0, 1);

				// delete item
				if (DictItemDel(d, key))
				{
					CursorSetPosition(103, 8);
					printf("ERROR: Item doesn't exist");
					CursorSetPosition(103, 9);
					printf("Press any key to continue...");
					i = _getch();
					i = 0;
				}
				CursorVisizible(FALSE);
				break;
			case 7:
				ScreenFill(0, 101, 6, 36, 5);
				CursorSetPosition(107, 6);
				printf("Get Item from Dictionary");
				CursorSetPosition(103, 8);
				printf("Key: ");

				CursorVisizible(TRUE);
				i = 0;
				// key init from stdin
				memset(key_, 0, 11);
				CursorSetPosition(108, 8);
				while (1)
				{
					key_[i] = _getch();
					if (key_[i] == ENTER)
						break;
					if (i == 10) continue;
					printf("%c", key_[i++]);
				}
				key = (char*)calloc(i + 1, sizeof(char));
				if (!key) exit(EXIT_FAILURE);
				memmove(key, key_, i);
				memset(&key[i], 0, 1);


				ITEM* find = DictItemGet(d, key);
				if (find == NULL)
				{
					CursorSetPosition(103, 8);
					printf("ERROR: Item doesn't exist");
					CursorSetPosition(103, 9);
					printf("Press any key to continue...");
					i = _getch();
					i = 0;
					CursorVisizible(FALSE);
					break;
				}
				else
				{
					TableClear();

					// Print key
					CursorSetPosition(3, 2);
					printf(" key: %-10s   type: ", find->key);

					// Print type and value
					if (find->type_val == STRING)
					{
						TextPrint(LightGreen, Black, "string  ");
						printf("  value: %-52s", find->str);
					}

					if (find->type_val == INTEGER)
					{
						TextPrint(LightBlue, Black, "integer ");
						printf("  value: %-52d", find->num);
					}

					if (find->type_val == _DOUBLE_)
					{
						TextPrint(LightRed, Black, "double  ");
						printf("  value: %-52lf", find->f_num);
					}

					CursorSetPosition(3, 3);
					ScreenSetColor(DarkGray, Black);
					for (int j = 0; j <= 93; j++) printf("-");
					ScreenSetColor(LightGray, Black);

					// Table's frame
					ScreenFill(223, 2,  1, 95, 1); // top horizontal
					ScreenFill(220, 2,  4, 95, 1); // bot horizontal
					ScreenFill(219, 2,  1,  1, 4); // 1 vertical
					ScreenFill(219, 20, 1,  1, 4); // 2 vertical
					ScreenFill(219, 36, 1,  1, 4); // 3 vertical
					ScreenFill(219, 97, 1,  1, 4); // 4 vertical


					CursorSetPosition(103, 9);
					printf("Press any key to continue...");
					i = _getch();
					i = 0;
					CursorVisizible(FALSE);
				}
				break;
			case 8:
				ScreenFill(0, 101, 6, 36, 5);
				TableClear();

				CursorSetPosition(103, 6);
				printf("List of dictionary's keys");

				// array of keys
				char** keys = DictKeysGet(d);
				c = d->actual_size;
				if (c > 265) c = 266;
				CursorSetPosition(103, 7);
				printf("First %d keys (all - %d)", c, d->actual_size);

				// print table of keys
				for (int i = 0; i < c; i++)
				{
					CursorSetPosition(3 + (13 * (i % 7)), 1 + (i / 7));
					printf("%s", keys[i]);
				}

				CursorSetPosition(103, 9);
				printf("Press any key to continue...");
				i = _getch();
				i = 0;
				free(keys);
				break;
			case 9:
				ScreenFill(0, 101, 6, 36, 5);
				CursorSetPosition(103, 6);
				printf("Choose type:");

				int choose = 1;
				do
				{
					CursorSetPosition(103, 7);
					if (choose == 1) TextPrint(Black, White, "1) String (char*)");
					else printf("1) String (char*)");
					CursorSetPosition(103, 8);
					if (choose == 2) TextPrint(Black, White, "2) Integer");
					else printf("2) Integer");
					CursorSetPosition(103, 9);
					if (choose == 3) TextPrint(Black, White, "3) Double");
					else printf("3) Double");

					BUTTON choose_b = _getch();

					if (choose_b == DOWN)
					{
						if (choose == 3) continue;
						choose++;
					}
					else if (choose_b == UP)
					{
						if (choose == 1) continue;
						choose--;
					}
					else if (choose_b == ENTER)
						break;

				} while (1);


				ScreenFill(0, 101, 6, 36, 5);
				TableClear();

				CursorSetPosition(103, 6);
				printf("List of dictionary's values");

				i = 0;
				int j = 0;
				if (choose == 1)
				{
					CursorSetPosition(103, 8);
					printf("Type: String (char*)");

					char** values = (char**)DictValsGet(d, 0);
					while (values[i] != NULL) i++;

					for (j = 0; j < i; j++)
					{
						CursorSetPosition(3 + (45 * (j % 2)), 1 + (j / 2));
						TextPrint(LightGreen, Black, values[j]);
						if (j == 75) break;
					}

					free(values);
				}
				else if (choose == 2)
				{
					CursorSetPosition(103, 8);
					printf("Type: Integer");

					int** values = (int**)DictValsGet(d, 1);
					while (values[i] != NULL) i++;

					for (j = 0; j < i; j++)
					{
						CursorSetPosition(3 + (20 * (j % 5)), 1 + (j / 5));
						ScreenSetColor(LightBlue, Black);
						printf("%d", *values[j]);
						ScreenSetColor(LightGray, Black);
						if (j == 189) break;
					}

					free(values);
				}
				else if (choose == 3)
				{
					CursorSetPosition(103, 8);
					printf("Type: Double");

					double** values = (double**)DictValsGet(d, 2);
					while (values[i] != NULL) i++;

					for (j = 0; j < i; j++)
					{
						CursorSetPosition(3 + (24 * (j % 4)), 1 + (j / 4));
						ScreenSetColor(LightRed, Black);
						printf("%.3lf", *values[j]);
						ScreenSetColor(LightGray, Black);
						if (j == 151) break;
					}

					free(values);
				}

				CursorSetPosition(103, 7);
				printf("First %d values (all - %d)", j, i);

				CursorSetPosition(103, 9);
				printf("Press any key to continue...");
				i = _getch();
				i = 0;
				break;
			}
			DictPrint(d, page);
			DictInfo(d, page);
		}

	} while (1);
}

void Fire(int x, int y, int c)
{
	ScreenSetColor(LightRed, Black);
	CursorSetPosition(x+c, y); printf("           (  .      )         ,.   ()   ( .      (\"   .      )  ( .      (\"   )         ,.   (   .      )         .      \"          ");
	CursorSetPosition(x-c, y+1); printf("        )  \"         ()   (\"     )     ( .      (\"   ( .      (\")     )'     , ()   (\"     )     )'   '        )  . (`     '`       ");
	CursorSetPosition(x+c, y+2); printf("               .  '   .   '  .  ')   ( .      (\"  .          .     ( .      (\" )  ; )   '  .  '  (\"))'     ,')  . (`     '`          ");
	CursorSetPosition(x+c, y+3); printf("    \"(, \")       (.   )  (      ( .      (\" )  ; )  ',   )   ( .      (\" )  ; )  ' (   ( (  )  (   ',    )  ; )   \"); (, (  \"  )\"  ");
	CursorSetPosition(x-c, y+4); printf("     .' ) ( . )    ,  ( ,     )   ( .      (\"))  )   ( .      (\"( ,     )   (   )   ( .     .      (\"'     ,')  . (`     '`          ");
	CursorSetPosition(x-c, y+5); printf("  ). , ( .   (  )\" (, ')  .')\")  (, ) '. )  ,. (')).,  (, )    (\"   )')).,  (, )    (\"   )'     ,'  ')  .' (, )    (\"   )        )  ");
	CursorSetPosition(x+c, y+6); printf(" (_,) . ), ) _) _,'\")  (, ) '. )  ,. ('))., (.   () (, ')\")  (, ) '. )  ,. ('))., (.)\")  (, ) '. )  ,. (')).,  )  ,. (')).,    ()   .'");
	CursorSetPosition(x-c, y+7); printf("(__\"., , ._'_.,)_(..,( . )_  _')_\"') (. _..( '.._\"., ,  )_  _')_\"') (. _..( '.._'_.,)_(..,( . )_  _')_') (._.,)_(..,(  _')_') (._.,)_(");
	ScreenSetColor(LightGray, Black);
}

void Intro()
{
	system("cls");
	CursorSetPosition(68, 29);
	ScreenSetColor(White, Black);
	printf("Loading..."); // Why? Because of life is not a sugar cake ><
	for (int j = 0; j < 32; j++)
	{
		CursorSetPosition(56 + j, 30);
		printf("%c", 219);
		Sleep(100);
	}
	Sleep(2000);
	system("cls");

	CursorVisizible(TRUE);

	CursorSetPosition(34, 25);
	const char* str1 = "You are a young librarian who burned accidentally all the books in the library.";
	for (int i = 0; i < 80; i++)
	{
		Sleep(50);
		printf("%c", str1[i]);
	}
	Sleep(500);

	CursorSetPosition(31, 26);
	str1 = "You wanted to undo it (ctrl+z), but it didn't work because you are not a programmer.";
	for (int i = 0; i < 85; i++)
	{
		Sleep(50);
		printf("%c", str1[i]);
	}
	Sleep(500);

	CursorSetPosition(42, 27);
	str1 = "Overall, you decided to rewrite some books on your own laptop.";
	for (int i = 0; i < 63; i++)
	{
		Sleep(50);
		printf("%c", str1[i]);
	}
	Sleep(500);

	CursorSetPosition(56, 29);
	str1 = "You started with dictionaries.....";
	for (int i = 0; i < 35; i++)
	{
		Sleep(50);
		printf("%c", str1[i]);
		if (i == 28) Sleep(2000);
	}
	Sleep(500);

	CursorVisizible(FALSE);

	short time = 100;
	ScreenSetColor(Yellow, Black);
	CursorSetPosition(14, 6);  printf("########  ######## ########     ###   "); Sleep(time);
	CursorSetPosition(14, 7);  printf("##     ## ##       ##     ##   ## ##  "); Sleep(time);
	CursorSetPosition(14, 8);  printf("##     ## ##       ##     ##  ##   ## "); Sleep(time);
	CursorSetPosition(14, 9);  printf("########  ######   ##     ## ##     ##"); Sleep(time);
	CursorSetPosition(14, 10); printf("##     ## ##       ##     ## #########"); Sleep(time);
	CursorSetPosition(14, 11); printf("##     ## ##       ##     ## ##     ##"); Sleep(time);
	CursorSetPosition(14, 12); printf("########  ######## ########  ##     ##"); Sleep(time);

	CursorSetPosition(54, 6);  printf("  ######  "); Sleep(time);
	CursorSetPosition(54, 7);  printf(" ##    ## "); Sleep(time);
	CursorSetPosition(54, 8);  printf(" ##       "); Sleep(time);
	CursorSetPosition(54, 9);  printf("  ######  "); Sleep(time);
	CursorSetPosition(54, 10); printf("       ## "); Sleep(time);
	CursorSetPosition(54, 11); printf(" ##    ## "); Sleep(time);
	CursorSetPosition(54, 12); printf("  ######  "); Sleep(time);

	CursorSetPosition(66, 6);  printf("########     ###     ######  ##     ## ##    ##  #######  #### "); Sleep(time);
	CursorSetPosition(66, 7);  printf("##     ##   ## ##   ##    ## ##     ## ##   ##  ##     ##  ##  "); Sleep(time);
	CursorSetPosition(66, 8);  printf("##     ##  ##   ##  ##       ##     ## ##  ##   ##     ##  ##  "); Sleep(time);
	CursorSetPosition(66, 9);  printf("########  ##     ##  ######  ######### #####    ##     ##  ##  "); Sleep(time);
	CursorSetPosition(66, 10); printf("##     ## #########       ## ##     ## ##  ##   ##     ##  ##  "); Sleep(time);
	CursorSetPosition(66, 11); printf("##     ## ##     ## ##    ## ##     ## ##   ##  ##     ##  ##  "); Sleep(time);
	CursorSetPosition(66, 12); printf("########  ##     ##  ######  ##     ## ##    ##  #######  #### "); Sleep(time);

	CursorSetPosition(62, 1);
	printf("EGOR TRUKHIN PRESENTS");

	CursorSetPosition(61, 14);
	printf("Game of the Year Edition");

	long i = 0;
	do
	{
		int c = 0;
		if (i % 3 == 1) c = 2;
		if (i % 3 == 2) c = -2;
		Fire(2, 32, c);

		if (i % 3 == 0)
			ScreenFill(0, 0, 32, 140, 1);

		ScreenSetColor(White, Black);
		CursorSetPosition(60, 34);
		printf("Press any button to play");

		i++;
		Sleep(300);

	} while (!_kbhit());
	int ch = _getch();

	for (int i = 0; i < 32; i++)
	{
		int c = 0;
		if (i % 3 == 1) c = 2;
		if (i % 3 == 2) c = -2;
		Fire(2, 32-i, c);
		Sleep(200); 
	}

	Sleep(1000);
	CursorSetPosition(0, 0);
	system("cls");
	ScreenSetColor(LightGray, Black);
}
