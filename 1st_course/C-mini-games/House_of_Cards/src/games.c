#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#include "games.h"
#include "interface.h"

typedef unsigned short score;

const char* ranks_str[] = { "Deuce", "Three", "Four",  "Five",
							"Six",   "Seven", "Eight", "Nine",
							"Ten",   "Jack",  "Queen", "King",
							"Ace",   "Joker", "Banner" };

const char* suits_str[] = { "Hearts", "Diamonds", "Clubs", "Spades" };

//-------------------------------------------------------------------------------------------

int TableGet(TABLE* table)
{

	if (table->game_id == CLOSE) return CLOSE;

	FieldClear();
	FieldTable();
	FieldPlayers(table);

	InfoClear();
	InfoTable(table);


	do
	{

		CursorSetPosition(113, 15);
		printf("Actions:");

		CursorSetPosition(98, 17);
		printf("0) Leave the table");
		CursorSetPosition(98, 18);
		printf("1) Start the game");
		CursorSetPosition(98, 19);
		printf("2) Show players' info");
		CursorSetPosition(98, 20);
		printf("3) Append player");
		CursorSetPosition(98, 21);
		printf("4) Kick last player");
		CursorSetPosition(98, 22);
		printf("5) Close the table");

		typedef enum {Leave, Start, Show_players, Append, Kick, Close} action;
		
		int act = _getch() - 48;
		int err = 0;

		if (act < 0 || act > 5)
			continue;


		switch (act)
		{
		case 0:
			return 0;
		case 2:
			InfoTablePlayers(table);
			InfoClear();
			InfoTable(table);
		break;
		case 3:
			err = TablePlayerAppend(table, PlayerCreate(NULL, (table->table_id + table->players_count)));
			if (err)
			{
				InfoClear();
				CursorSetPosition(109, 8);
				printf("Not enough place");
				CursorSetPosition(107, 9);
				printf("to append new player");
				CursorSetPosition(103, 11);
				printf("Press any button to continue");
				err = _getch();
				InfoClear();
				InfoTable(table);
			}
			else
			{
				InfoTable(table);
				FieldPlayers(table);
			}
		break;
		case 4:
			err = TablePlayerKick(table);
			if (err)
			{
				InfoClear();
				CursorSetPosition(108, 9);
				printf("The table is empty");
				CursorSetPosition(103, 11);
				printf("Press any button to continue");
				err = _getch();
				InfoClear();
				InfoTable(table);
			}
			else
			{
				InfoTable(table);
				FieldPlayers(table);
			}
		break;
		case 1:
			if (table->players_count < 3)
			{
				InfoClear();
				CursorSetPosition(99, 9);
				printf("Not enough players to start the game");
				CursorSetPosition(103, 11);
				printf("Press any button to continue");
				err = _getch();
				InfoClear();
				InfoTable(table);
				break;
			}
			switch (table->game_id)
			{
			case TWENTY_ONE:
				Game_TwentyOne(table);
			break;
			case BLACKJACK:
				Game_Blackjack(table);
			}

			// End of game on current table
			for (int i = 0; i < table->players_count; i++)
				PlayerKill(table->players[i]);
			table->game_id = CLOSE;
			return 0;
		break;
		case 5:
			// Just close the table without game
			for (int i = 0; i < table->players_count; i++)
				PlayerKill(table->players[i]);
			table->game_id = CLOSE;
			return 0;			
		}

	} while (1);
	
	return 0;
}

//-------------------------------------------------------------------------------------------

void InfoTable(TABLE* table)
{		

	CursorSetPosition(113, 6);
	printf("Table #%d", table->table_id);

	if (table->game_id == CLOSE)
	{
		CursorSetPosition(110, 8);
		printf("Table is closed");
		CursorSetPosition(96, 10);
		for (int i = 0; i < 44; i++)
			printf("%c", 223);
		return;
	}

	CursorSetPosition(110, 9);
	printf("Deck: %d cards", table->deck->size);

	CursorSetPosition(111, 10);
	printf("Gamblers: %d ", table->players_count);

	CursorSetPosition(108, 8);
	printf("Gambl: ");

	switch (table->game_id)
	{
	case TWENTY_ONE:
		printf("Twenty-one");
		break;
	case BLACKJACK:
		printf("Blackjack");
		break;
	case TEXAS:
		printf("Texas Hold'em");
		break;
	case FOOL:
		printf("Durak (Fool)");
		break;
	default:
		break;
	}

	ScreenClear(0, 98, 12, 40, 1);
	if (table->players_count > 2)
	{
		CursorSetPosition(105, 12);
		printf("Players are ready to play");
	}
	else
	{
		CursorSetPosition(99, 12);
		printf("Not enough players to start the game");
	}


	CursorSetPosition(96, 14);
	for (int i = 0; i < 44; i++)
		printf("%c", 223);

}

void InfoTablePlayers(TABLE* table)
{

	InfoClear();
	do
	{

		CursorSetPosition(106, 6);
		printf("Players: (on clockwise)");

		CursorSetPosition(98, 8);
		for (int i = 0; i < table->players_count; i++)
		{
			if ((i) % 3 == 0)
				CursorSetPosition(98, 8 + (i / 3));
			printf("%d) %-12s", i + 1, table->players[i]->name);
		}

		if (table->players_count != 0)
		{
			CursorSetPosition(102, 12);
			printf("Choose player(1-%d) to see info", table->players_count);
			CursorSetPosition(107, 13);
			printf("or press 0 to exit");
		}
		else
		{
			CursorSetPosition(108, 9);
			printf("The table is empty");
			CursorSetPosition(110, 13);
			printf("Press 0 to exit");
		}

		int num = _getch() - 48;
		if (num == 0)
			return;
		if (num < 0 || num > table->players_count)
			continue;
		else
		{
			InfoTablePlayerOne(table->players[num - 1]);
			InfoClear();
		}
	} while (1);

}

void InfoTablePlayerOne(PLAYER* player)
{
	InfoClear();

	CursorSetPosition(110, 6);
	printf("Player \"%s\"", player->name);
	CursorSetPosition(112, 7);
	printf("Cards: %d", player->hand->size);

	if (player->hand->size != 0)
	{
		CursorSetPosition(100, 8);
		printf("Hand:");
		CursorSetPosition(100, 9);
		for (int i = 0; i < player->hand->size; i++)
		{
			CursorSetPosition(100, 9 + i);
			printf("%2d)%6s of %-8s  ", i + 1, ranks_str[player->hand->cards[i]->rank],
				                               suits_str[player->hand->cards[i]->suit]);

		}
	}
		
	CursorSetPosition(109, 8 + (player->hand->size) / 2 + 4);
	printf("Press 0 to exit");
	int num = _getch() - 48;
	while (num) num = _getch() - 48;
}

//-------------------------------------------------------------------------------------------
int x = 18, y = 6; // table's coordinates

void FieldTable()
{

	// Horizontal top
	CursorSetPosition(x, y);
	for (int i = 0; i < 60; i++)
		printf("%c", 220);

	// Horizontal bottom
	CursorSetPosition(x, y + 26);
	for (int i = 0; i < 60; i++)
		printf("%c", 220);

	// Vertical left
	CursorSetPosition(x, y + 1);
	for (int i = 0; i < 27; CursorSetPosition(x, y + 1 + i), i++)
		printf("%c", 219);

	// Vertical ritght
	CursorSetPosition(x + 59, y + 1);
	for (int i = 0; i < 27; CursorSetPosition(x + 59, y + 1 + i), i++)
		printf("%c", 219);

	// Corners of table
	ScreenClear(219, x-1,  y,    3, 2);
	ScreenClear(219, x+57, y,    3, 2);
	ScreenClear(219, x-1,  y+26, 3, 2);
	ScreenClear(219, x+57, y+26, 3, 2);

	// Deck
	ScreenClear(219, x + 27, y + 23, 7, 3);
	ScreenClear('D', x + 29, y + 24, 0, 0);
	ScreenClear('E', x + 30, y + 24, 0, 0);
	ScreenClear('C', x + 31, y + 24, 0, 0);
	ScreenClear('K', x + 32, y + 24, 0, 0);


}

void FieldPlayers(TABLE* table)
{
	PLAYER** players = table->players;
	if (!players) exit(EXIT_FAILURE);

	// Vertical "Chelovechki" left

	for (int i = 0; i < 3; i++)
	{

		ScreenClear(0, x - 12, y + 4 + (8 * (2 - i)), 7, 4);

		if (players[0] == NULL && i == 0) continue;
		if (players[1] == NULL && i == 1) continue;
		if (players[2] == NULL && i == 2) continue;

		CursorSetPosition(x - 12, y + 5 + (8 * (2 - i))); printf(" #%d ", i + 1);
		CursorSetPosition(x - 8, y + 4 + (8 * (2 - i))); printf(" () ");
		CursorSetPosition(x - 8, y + 5 + (8 * (2 - i))); printf("/||\\");
		CursorSetPosition(x - 8, y + 6 + (8 * (2 - i))); printf(" /\\");
	}

	// Vertical "Chelovechki" right
	for (int i = 0; i < 3; i++)
	{

		ScreenClear(0, x - 8 + 71, y + 4 + (8 * i), 6, 4);

		if (players[6] == NULL && i == 0) continue;
		if (players[7] == NULL && i == 1) continue;
		if (players[8] == NULL && i == 2) continue;

		CursorSetPosition(x - 4 + 71, y + 5 + (8 * i)); printf(" #%d ", i + 7);
		CursorSetPosition(x - 8 + 71, y + 4 + (8 * i)); printf(" () ");
		CursorSetPosition(x - 8 + 71, y + 5 + (8 * i)); printf("/||\\");
		CursorSetPosition(x - 8 + 71, y + 6 + (8 * i)); printf(" /\\");
	}

	// Horizontal "Chelovechki"
	for (int i = 0; i < 3; i++)
	{

		ScreenClear(0, x + 9 + (20 * i), y - 5, 5, 5);

		if (players[3] == NULL && i == 0) continue;
		if (players[4] == NULL && i == 1) continue;
		if (players[5] == NULL && i == 2) continue;

		CursorSetPosition(x + 9 + (20 * i), y - 5 ); printf(" #%d ", i + 4);
		CursorSetPosition(x + 9 + (20 * i), y - 4 ); printf(" () ");
		CursorSetPosition(x + 9 + (20 * i), y - 3 ); printf("/||\\");
		CursorSetPosition(x + 9 + (20 * i), y - 2 ); printf(" /\\");
	}

	// Name of players on clockwise
	if (players[0] != NULL) { CursorSetPosition(x - 8, y + 7 + (8 * 2)); printf("%s", players[0]->name); };
	if (players[1] != NULL) { CursorSetPosition(x - 8, y + 7 + (8 * 1)); printf("%s", players[1]->name); };
	if (players[2] != NULL) { CursorSetPosition(x - 8, y + 7 + (8 * 0)); printf("%s", players[2]->name); };

	if (players[3] != NULL) { CursorSetPosition(x + 9 + 0 , y - 1); printf("%s", players[3]->name); };
	if (players[4] != NULL) { CursorSetPosition(x + 9 + 20, y - 1); printf("%s", players[4]->name); };
	if (players[5] != NULL) { CursorSetPosition(x + 9 + 40, y - 1); printf("%s", players[5]->name); };

	if (players[6] != NULL) { CursorSetPosition(x - 8 + 71, y + 7 + (8 * 0)); printf("%s", players[6]->name); };
	if (players[7] != NULL) { CursorSetPosition(x - 8 + 71, y + 7 + (8 * 1)); printf("%s", players[7]->name); };
	if (players[8] != NULL) { CursorSetPosition(x - 8 + 71, y + 7 + (8 * 2)); printf("%s", players[8]->name); };

}

void FieldCardTake(TABLE* table, int player)
{
	int speed = 30;

	int j = 10;
	if (player == 4) j = 20;

	// Move card from deck to center of the table
	for (int i = 0; i < j; i++)
	{

		if (i == 0)
		{
			CursorSetPosition(x + 29, y + 22 - i);
			printf("%c%c%c%c", 178, 178, 178, 178);
			Sleep(speed);
			ScreenClear(0, x + 29, y + 22 - i, 3, 1);
		}

		if (i == 1)
		{
			CursorSetPosition(x + 29, y + 22 - i);
			printf("%c%c%c%c", 178, 178, 178, 178);
			CursorSetPosition(x + 29, y + 23 - i);
			printf("%c%c%c%c", 178, 178, 178, 178);
			Sleep(speed);
			ScreenClear(0, x + 29, y + 22 - i, 3, 2);
		}

		if (i > 1)
		{
			CursorSetPosition(x + 29, y + 22 - i);
			printf("%c%c%c%c", 178, 178, 178, 178);
			CursorSetPosition(x + 29, y + 23 - i);
			printf("%c%c%c%c", 178, 178, 178, 178);
			CursorSetPosition(x + 29, y + 24 - i);
			printf("%c%c%c%c", 178, 178, 178, 178);
			Sleep(speed);
			if (i == 10) Sleep(speed);
			ScreenClear(0, x + 29, y + 22 - i, 3, 3);
		}

	}

	if (player == 4) return;

	// Move card from center of the table to current player
	if (player < 3)
	{
		j = 17;
		if (player == 1) j = 27;
		for (int i = 0; i < j; i++)
		{
			CursorSetPosition(x + 29 - i, y + 13);
			printf("%c%c%c%c", 178, 178, 178, 178);
			CursorSetPosition(x + 29 - i, y + 14);
			printf("%c%c%c%c", 178, 178, 178, 178);
			CursorSetPosition(x + 29 - i, y + 15);
			printf("%c%c%c%c", 178, 178, 178, 178);
			Sleep(speed);
			ScreenClear(0, x + 29 - i, y + 13, 3, 3);
		}

		j = 1;
		if (player == 0 || player == 2)
			for (int i = 0; i < 9; i++)
			{
				if (player == 2) j = -1;
				CursorSetPosition(x + 12 - i, y + 13 + (i * j));
				printf("%c%c%c%c", 178, 178, 178, 178);
				CursorSetPosition(x + 12 - i, y + 14 + (i * j));
				printf("%c%c%c%c", 178, 178, 178, 178);
				CursorSetPosition(x + 12 - i, y + 15 + (i * j));
				printf("%c%c%c%c", 178, 178, 178, 178);
				Sleep(speed);
				ScreenClear(0, x + 12 - i, y + 13 + (i * j), 3, 3);
			}

	}

	if (player > 5)
	{
		j = 17;
		if (player == 7) j = 25;
		for (int i = 0; i < j; i++)
		{
			CursorSetPosition(x + 29 + i, y + 13);
			printf("%c%c%c%c", 178, 178, 178, 178);
			CursorSetPosition(x + 29 + i, y + 14);
			printf("%c%c%c%c", 178, 178, 178, 178);
			CursorSetPosition(x + 29 + i, y + 15);
			printf("%c%c%c%c", 178, 178, 178, 178);
			Sleep(speed);
			ScreenClear(0, x + 28 + i, y + 13, 4, 3);
		}

		j = 1;
		if (player == 6 || player == 8)
			for (int i = 0; i < 9; i++)
			{
				if (player == 6) j = -1;
				CursorSetPosition(x + 46 + i, y + 13 + (i * j));
				printf("%c%c%c%c", 178, 178, 178, 178);
				CursorSetPosition(x + 46 + i, y + 14 + (i * j));
				printf("%c%c%c%c", 178, 178, 178, 178);
				CursorSetPosition(x + 46 + i, y + 15 + (i * j));
				printf("%c%c%c%c", 178, 178, 178, 178);
				Sleep(speed);
				ScreenClear(0, x + 45 + i, y + 13 + (i * j), 4, 3);
			}

	}

	j = 1;
	if (player < 6 && player > 2)
	{
		if (player == 3) j = -1;
		for (int i = 0; i < 12; i++)
		{
			CursorSetPosition(x + 29 + (i * j), y + 13 - i);
			printf("%c%c%c%c", 178, 178, 178, 178);
			CursorSetPosition(x + 29 + (i * j), y + 14 - i);
			printf("%c%c%c%c", 178, 178, 178, 178);
			CursorSetPosition(x + 29 + (i * j), y + 15 - i);
			printf("%c%c%c%c", 178, 178, 178, 178);
			Sleep(speed);
			ScreenClear(0, x + 28 + (i * j), y + 12 - i, 4, 4);
		}
	}

}

//-------------------------------------------------------------------------------------------


void Game_TwentyOne(TABLE* table)
{
	InfoClear();

	score* scores = (score*)calloc(table->players_count, sizeof(score));
	if (!scores) exit(EXIT_FAILURE);

	score max_score = 0;
	do
	{
		InfoTable(table);
		CursorSetPosition(105, 12);
		printf("                         ");
		CursorSetPosition(110, 12);
		printf("Game in process");

		// Players and score block
		CursorSetPosition(106, 15);
		printf("Players");

		CursorSetPosition(122, 15);
		printf("Scores");

		for (int i = 0; i < table->players_count; i++)
		{
			CursorSetPosition(106, 16 + i);
			printf("%d) %s", (i + 1), table->players[i]->name);

			CursorSetPosition(124, 16 + i);
			printf("%2d", scores[i]);
		}

		CursorSetPosition(96, 17 + table->players_count);
		for (int i = 0; i < 44; i++)
			printf("%c", 223);


		if (max_score < 21)
		{
			ScreenClear(0, 97, 18 + table->players_count, 139 - 97, 40 - (19 + table->players_count));

			// Interactive block
			CursorSetPosition(110, 19 + table->players_count);
			printf("Game's actions:");

			CursorSetPosition(98, 21 + table->players_count);
			printf("1) Give one card to everyone");

			CursorSetPosition(98, 22 + table->players_count);
			printf("2) Show players' info");

			int action = _getch() - 48;
			switch (action)
			{
			case 1:
				ScreenClear(0, 97, 18 + table->players_count, 139 - 97, 40 - (19 + table->players_count));
				CursorSetPosition(107, 19 + table->players_count);
				printf("Handing out cards");
				CursorSetPosition(107, 20 + table->players_count);
				printf("  Keep waiting...");
				break;
			case 2:
				InfoTablePlayers(table);
				InfoClear();
				continue;
			default:
				continue;
			}
		}
		else
		{
			ScreenClear(0, 97, 18 + table->players_count, 139 - 97, 40 - (19 + table->players_count));
			CursorSetPosition(105, 12);
			printf("                         ");
			CursorSetPosition(112, 12);
			printf("Game over");
			break;
		}

		// Give card to everyone
		for (int i = 0; i < table->players_count; i++)
		{
			FieldCardTake(table, i); // Animation of taking card by some player
			CardTake(table->players[i], table->deck);
		}

		// Score check
		for (int i = 0; i < table->players_count; i++)
		{
			PLAYER* player = table->players[i];
			score cur_score = 0;
			for (int j = 0; j < player->hand->size; j++)
			{
				int rank = player->hand->cards[j]->rank;   // Jack = 2
				if (rank < JACK)                            // Queen = 3
					cur_score += rank + 2;                  // King = 4
				else if (rank < ACE)                        // Ace = 11
					cur_score += rank - 7;                  // Other ones (6 - 10) - accordingly
				else
					cur_score += ACE - 1;
			}

			scores[i] = cur_score;
			if (max_score < cur_score)
				max_score = cur_score;

		}
	} while (1);

	// Choose the winner
	int win = 0;
	score min_sub = 21;
	PLAYER* winner = NULL;
	for (int i = 0; i < table->players_count; i++)
	{
		PLAYER* player = table->players[i];
		score cur_score = scores[i];

		cur_score = abs(21 - cur_score);
		if (cur_score < min_sub)
		{
			win = i;
			min_sub = cur_score;
			winner = player;
		}
	}

	// Current game over block

	CursorSetPosition(106, 20 + table->players_count);
	if (winner != NULL)
		printf("Winner is \"%s\" (#%d)", winner->name, win + 1);
	else
		printf("  No one is a winner");

	CursorSetPosition(104, 24 + table->players_count);
	printf("Press any button to exit");

	min_sub = _getch();

	free(scores);
	InfoClear();
}

void Game_Blackjack(TABLE* table)
{
	InfoClear();

	score* scores = (score*)calloc(table->players_count, sizeof(score));
	if (!scores) exit(EXIT_FAILURE);

	score max_score = 0;
	do
	{
		InfoTable(table);
		CursorSetPosition(105, 12);
		printf("                         ");
		CursorSetPosition(110, 12);
		printf("Game in process");

		// Players and score block
		CursorSetPosition(106, 15);
		printf("Players");

		CursorSetPosition(122, 15);
		printf("Scores");

		for (int i = 0; i < table->players_count; i++)
		{
			CursorSetPosition(106, 16 + i);
			printf("%d) %s", (i + 1), table->players[i]->name);

			CursorSetPosition(124, 16 + i);
			printf("%2d", scores[i]);
		}

		CursorSetPosition(96, 17 + table->players_count);
		for (int i = 0; i < 44; i++)
			printf("%c", 223);


		if (max_score < 21)
		{
			// Interactive block
			CursorSetPosition(110, 19 + table->players_count);
			printf("Game's actions:");

			CursorSetPosition(98, 21 + table->players_count);
			printf("1) Give one card to everyone");

			CursorSetPosition(98, 22 + table->players_count);
			printf("2) Show players' info");

			int action = _getch() - 48;
			switch (action)
			{
			case 1:
				break;
			case 2:
				InfoTablePlayers(table);
				InfoClear();
				continue;
			default:
				continue;
			}
		}
		else
		{
			ScreenClear(0, 97, 18 + table->players_count, 139 - 97, 40 - (19 + table->players_count));
			CursorSetPosition(105, 12);
			printf("                         ");
			CursorSetPosition(112, 12);
			printf("Game over");
			break;
		}

		// Give card to everyone
		for (int i = 0; i < table->players_count; i++)
		{
			FieldCardTake(table, i); // Animation of taking card by some player
			CardTake(table->players[i], table->deck);
		}

		// Score check
		for (int i = 0; i < table->players_count; i++)
		{
			PLAYER* player = table->players[i];
			score cur_score = 0;
			for (int j = 0; j < player->hand->size; j++)
			{
				int rank = player->hand->cards[j]->rank;   // Jack = 10
				if (rank < JACK)                            // Queen = 10
					cur_score += rank + 2;                  // King = 10
				else if (rank < ACE)                        // Ace = 1
					cur_score += 10;                        // Other ones (2 - 10) - accordingly
				else
					cur_score += 1;
			}

			scores[i] = cur_score;
			if (max_score < cur_score)
				max_score = cur_score;

		}
	} while (1);

	// Choose the winner
	int win = 0;
	score min_sub = 21;
	PLAYER* winner = NULL;
	for (int i = 0; i < table->players_count; i++)
	{
		PLAYER* player = table->players[i];
		score cur_score = scores[i];

		cur_score = abs(21 - cur_score);
		if (cur_score < min_sub)
		{
			win = i;
			min_sub = cur_score;
			winner = player;
		}
	}

	// Current game over block
	CursorSetPosition(106, 20 + table->players_count);
	if (winner != NULL)
		printf("Winner is \"%s\" (#%d)", winner->name, win + 1);
	else
		printf("  No one is a winner");

	CursorSetPosition(104, 24 + table->players_count);
	printf("Press any button to exit");

	min_sub = _getch();

	free(scores);
	InfoClear();
}
