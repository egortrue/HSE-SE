#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#include "interface.h"
#include "cards.h"
#include "games.h"

void ConsoleRemake()
{
	// I don't know what exactly there happen xD
	// (PLEASE DO NOT CHANGE IT)

	// Set console box size (140x40) and make console box unsizable
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	system("mode con cols=140 lines=40");

	// Invisizible cursor
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void CursorSetPosition(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ScreenClear(int ch, int x, int y, int xl, int yl)
{
	CursorSetPosition(x, y);
	for (int i = 0; i <= yl; i++)
	{
		for (int j = 0; j <= xl; j++)
			printf("%c", ch);
		CursorSetPosition(x, y+i);
	}
	CursorSetPosition(x, y);
}

// Often used type of ScreenClear()
void FieldClear() { ScreenClear(0, 0, 0, 94, 40); }
void InfoClear()  { ScreenClear(0, 96, 6, 41, 34); }

void Intro()
{
	system("cls");
	CursorSetPosition(66, 29);
	printf("Loading..."); // Why? Because of life is not a sugar cake ><
	for (int j = 0; j < 32; j++)
	{
		CursorSetPosition(54 + j, 30);
		printf("%c", 219);
		Sleep(100);
	}
	Sleep(2000);
	system("cls");

	                                  // old version
	//
	//for (int i = 0; i < 34; i++)
	//{
	//	CursorSetPosition(58, 34 - i);
	//	puts("");
	//	puts("       __    __   ______   __    __     ______. _______      ______   _______    ______      ___     .______     _______      ______.");
	//	puts("      |  |  |  | /  __  \\ |  |  |  |   /      ||   ____|    /  __  \\ |   ____|  /      |    /   \\    |   _  \\   |       \\    /      |");
	//	puts("      |  |__|  ||  |  |  ||  |  |  |  |   (---`|  |__      |  |  |  ||  |__    |  ,----'   /  ^  \\   |  |_)  |  |  .--.  |  |   (---`");
	//	puts("      |   __   ||  |  |  ||  |  |  |   \\   \\   |   __|     |  |  |  ||   __|   |  |       /  /_\\  \\  |      /   |  |  |  |   \\   \\   ");
	//	puts("      |  |  |  ||  `--'  ||  `--'  |.---)   |  |  |____    |  `--'  ||  |      |  `----. /  _____  \\ |  |\\  \\--.|  '--'  |.---)   |  ");
	//	puts("      |__|  |__| \\______/  \\______/ |______/   |_______|    \\______/ |__|       \\______|/__/     \\__\\| _| `.___||_______/ |______/   ");
	//	Sleep(100);
	//	if (i != 33)
	//		system("cls");
	//}
	
	Sleep(2000);
	CursorSetPosition(58, 1);
	const char* logo = "EGOR TRUKHIN PRESENTS";
	for (int i = 0; i < 22; i++)
	{
		Sleep(200);
		printf("%c", logo[i]);
	}
	Sleep(2000);



	int t = 200;
	CursorSetPosition(0, 2);
	puts("       __    __   ______   __    __     ______. _______   "); Sleep(t);
	puts("      |  |  |  | /  __  \\ |  |  |  |   /      ||   ____| "); Sleep(t);
	puts("      |  |__|  ||  |  |  ||  |  |  |  |   (---`|  |__     "); Sleep(t);
	puts("      |   __   ||  |  |  ||  |  |  |   \\   \\   |   __|  "); Sleep(t);
	puts("      |  |  |  ||  `--'  ||  `--'  |.---)   |  |  |____   "); Sleep(t);
	puts("      |__|  |__| \\______/  \\______/ |______/   |_______|"); Sleep(t);

	CursorSetPosition(59, 2);
	puts("  ______   _______  "); Sleep(t); CursorSetPosition(59, 3);
	puts(" /  __  \\ |   ____|"); Sleep(t); CursorSetPosition(59, 4);
	puts("|  |  |  ||  |__    "); Sleep(t); CursorSetPosition(59, 5);
	puts("|  |  |  ||   __|   "); Sleep(t); CursorSetPosition(59, 6);
	puts("|  `--'  ||  |      "); Sleep(t); CursorSetPosition(59, 7);
	puts(" \\______/ |__|     "); Sleep(t); CursorSetPosition(59, 8);

	CursorSetPosition(80, 2);
	puts("  ______      ___     .______     _______      ______.   "); Sleep(t); CursorSetPosition(80, 3);
	puts(" /      |    /   \\    |   _  \\   |       \\    /      |"); Sleep(t); CursorSetPosition(80, 4);
	puts("|  ,----'   /  ^  \\   |  |_)  |  |  .--.  |  |   (---`  "); Sleep(t); CursorSetPosition(80, 5);
	puts("|  |       /  /_\\  \\  |      /   |  |  |  |   \\   \\  "); Sleep(t); CursorSetPosition(80, 6);
	puts("|  `----. /  _____  \\ |  |\\  \\--.|  '--'  |.---)   |  "); Sleep(t); CursorSetPosition(80, 7);
	puts(" \\______|/__/     \\__\\| _| `.___||_______/ |______/   "); Sleep(t); CursorSetPosition(80, 8);


	Sleep(2000);
	CursorSetPosition(45, 18);
	printf("You are the Manager of the \"House of Cards\" casino.");

	CursorSetPosition(48, 19);
	printf("Your mission to control gambles and gamblers.");

	CursorSetPosition(63, 20);
	printf("Good Luck");


	Sleep(2000);
	CursorSetPosition(59, 30);
	printf("Press SPACE to play");
	int space = 1;

	while (space) space = _getch() - 32;

	CursorSetPosition(0, 0);
	system("cls");
}

void Logo()
{

	CursorSetPosition(98, 0); puts("    _     _  _   _  _   _     _  _   _ ");
	CursorSetPosition(98, 1); puts("| || || ||  |   | ||   |  /\\ | \\| \\ | ");
	CursorSetPosition(98, 2); puts("|=|| || | \\ |-  | ||-  | /__\\|_/|  | \\");
	CursorSetPosition(98, 3); puts("| ||_||_\\ _||_  |_||   |_|  || \\|_/  _|");

	CursorSetPosition(95, 5);
	for (int i = 0; i < 45; i++)
		printf("%c", 223);

	CursorSetPosition(95, 0);
	for (int i = 0; i < 6; CursorSetPosition(95, i++))
		printf("%c", 219);
}

void Menu()
{

	CursorSetPosition(95, 5);
	for (int i = 0; i < 45; i++)
		printf("%c", 223);

	CursorSetPosition(95, 5);
	for (int i = 6; i < 41; CursorSetPosition(95, i++))
		printf("%c", 219);

	TABLE** tables = (TABLE**)calloc(2, sizeof(TABLE*));
	if (!tables) exit(EXIT_FAILURE);

	tables[0] = TableCreate(1);
	tables[1] = TableCreate(2);

	TableSet(tables[0], TWENTY_ONE);
	TableSet(tables[1], BLACKJACK);

	do
	{

		int err = 0;
		for (int i = 1; i < 5; i++)
		{
			CursorSetPosition(17 * i, 16);  printf(" ___________");
			CursorSetPosition(17 * i, 17); 	printf("|           |");
			CursorSetPosition(17 * i, 18); 	printf("|   Table   |");
			CursorSetPosition(17 * i, 19); 	printf("|    #%d     |", i);
			CursorSetPosition(17 * i, 20); 	printf("|___________|");
		}

		CursorSetPosition(18 * 1, 21); printf("%s", "Twenty  One");
		CursorSetPosition(18 * 2, 21); printf("%s", "BlackJack");
		CursorSetPosition(18 * 3 - 3, 21); printf("%s", "Texas Hold'em");
		CursorSetPosition(18 * 4, 21); printf("%s", "Durak");

		CursorSetPosition(110, 7);
		printf("Choose the table");
		CursorSetPosition(106, 8);
		printf("to start the game: (1-4)");

		CursorSetPosition(104, 9);
		printf("or press 0 to exit the game");


		int g = _getch() - 48;
		if (g == 0)
			break;
		if (g > 4 || g < 0)
			continue;
		switch (g)
		{
		case 1:
			err = TableGet(tables[0]); // Begin of game

			if (err)
			{
				InfoClear();
				CursorSetPosition(106, 7);
				printf("The table #1 is closed");

				CursorSetPosition(103, 10);
				printf("Press any button to continue");
				err = _getch();
			}

			FieldClear();
			InfoClear();
			break;
		case 2:
			err = TableGet(tables[1]); // Begin of game

			if (err)
			{
				InfoClear();
				CursorSetPosition(106, 7);
				printf("The table #2 is closed");

				CursorSetPosition(103, 10);
				printf("Press any button to continue");
				err = _getch();
			}

			FieldClear();
			InfoClear();
			break;
		case 3:
			InfoClear();
			CursorSetPosition(100, 7);
			printf("The Texas Hold'em is not available");
			CursorSetPosition(103, 8);
			printf("Please choose another table");
			CursorSetPosition(103, 10);
			printf("Press any button to continue");
			err = _getch();
			InfoClear();
			break;
		case 4:
			InfoClear();
			CursorSetPosition(104, 7);
			printf("The Durak is not available");
			CursorSetPosition(103, 8);
			printf("Please choose another table");
			CursorSetPosition(103, 10);
			printf("Press any button to continue");
			err = _getch();
			InfoClear();
			break;
		default:
			break;
		}
	} while (1);


	TableDestroy(tables[0]);
	TableDestroy(tables[1]);
	free(tables);

}

void GameOver()
{
	system("cls");
	Logo();
	CursorSetPosition(65, 18);
	puts("GAME OVER");
	CursorSetPosition(0, 36);  // 4 rows for default debug output info
}
