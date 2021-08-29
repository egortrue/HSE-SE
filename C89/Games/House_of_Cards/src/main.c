#include "main.h"
int main()
{
	// Size of console box is approximately 1120x670 pixels (140 columns and 40 rows)
	ConsoleRemake();
	Intro();
	Logo();
	Menu();
	GameOver();
	_getch();
	return 0;
}
