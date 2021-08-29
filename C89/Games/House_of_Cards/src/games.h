#ifndef GAMES_H
#define GAMES_H

// This header contains some game functions
// which paint games' animation 
// and print common information about current game 

#include "cards.h"

int TableGet(TABLE* table);

// Info block (under logo)
void InfoTable          (TABLE* table);
void InfoTablePlayers   (TABLE* table);
void InfoTablePlayerOne (PLAYER* player);

// Game field (paint and animation)
void FieldTable();
void FieldPlayers(TABLE* table);
void FieldCardTake(TABLE* table, int player);

// Games
void Game_TwentyOne (TABLE* table);
void Game_Blackjack (TABLE* table);

#endif
