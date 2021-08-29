#ifndef INTERFACE_H
#define  INTERFACE_H

// This header contains some functions
// which make some changes in console
// and print common interface

// Console box size and invizible cursor
void ConsoleRemake();

// Set position cursor on the screen. Start of typing in this place
void CursorSetPosition(int x, int y);

// Clear part(a block) of screen ((x, y) - begin pos, xl - width, yl - height)
void ScreenClear(int ch, int x, int y, int xl, int yl);
void FieldClear();
void InfoClear();

// Initial animation
void Intro();

// Main interface
void Logo();
void Menu();
void GameOver();

#endif
