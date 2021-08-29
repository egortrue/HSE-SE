#ifndef INTERFACE_H

#define INTERFACE_H
#include "dict.h"

enum ConsoleColor {
	Black = 0,     // Default color of background
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7, // Default color of text
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

typedef enum button
{
	ENTER = 13,
	UP = 72,
	RIGHT = 77,
	LEFT = 75,
	DOWN = 80,
	DEL = 83,
	ESC = 27,
} BUTTON;

// Console box size, scroll bar and invizible cursor
void ConsoleRemake();

// Set position cursor on the screen. Start of typing in this place
void CursorSetPosition(int x, int y);

// Clear part(a block) of screen ((x, y) - begin pos, xl - width, yl - height)
void ScreenFill(int ch, int x, int y, int xl, int yl);
void TableClear();
void InfoClear();
void ActionClear();

// Set color fot text and background for next typing symbols
void ScreenSetColor(int text, int bg);

// BookShelf
void BookShelf(DICT** bookshelf);

// Print string with current color
void TextPrint(int text, int bg, char* str);

// Common info about dictionary
void  DictInfo(DICT* d, int page);

// Full table with all keys and values
void  DictPrint(DICT* d, int page);

// Actions with dictionary
void  Dictionary(DICT* d);

void Intro();

#endif