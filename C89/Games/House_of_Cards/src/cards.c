#include <stdlib.h>
#include <time.h>
#include "cards.h"

const char* names_str[] = { "John", "Ivan", "Ilya", "Lena", "Jack",
							"Erik", "Joey", "Jake", "Mike", "Hank",
							"Niko", "Kyle", "Cody", "Abel", "Luis",
							"Kate", "Luke", "Sami", "Alen", "Cris"
							"Igor", "Egor", "Jane", "Tany", "Fisk" };

//-------------------------------------------------------------------------------------------

DECK* DeckCreate(GAME game_id)
{

	int cards_num   = 0;
	RANK rank_first = TWO;
	RANK rank_last  = BANNER;

	switch (game_id)
	{
	case EMPTY:
		return NULL;
	case TEXAS:
		cards_num = 52;
		rank_last = ACE;
		break;
	case FOOL:
		cards_num  = 36;
		rank_first = SIX;
		rank_last  = ACE;
		break;
	case TWENTY_ONE:
		cards_num  = 36;
		rank_first = SIX,
		rank_last  = ACE;
		break;
	case BLACKJACK:
		cards_num = 52;
		rank_last = ACE;
		break;
	default:
		exit(EXIT_FAILURE);
	}

	DECK* deck = (DECK*)malloc(sizeof(DECK));
	if (!deck) exit(EXIT_FAILURE);

	deck->size  = cards_num;
	deck->cards = (CARD**)calloc(cards_num, sizeof(CARD*));
	if (!deck->cards) exit(EXIT_FAILURE);

	for (int i = 0; i < cards_num; i++)
	{
		deck->cards[i] = (CARD*)malloc(sizeof(CARD));
		if (!deck->cards[i]) exit(EXIT_FAILURE);

		deck->cards[i]->rank = rank_first + i % (rank_last - rank_first + 1);
		deck->cards[i]->suit = rank_first + i / (rank_last - rank_first + 1);
	}


	return deck;
}

CARD* DeckPop(DECK* deck)
{
	CARD* card = deck->cards[--(deck->size)];
	deck->cards[(deck->size)] = NULL;
	return card;
}

void DeckShuffle(DECK* deck)
{
	srand((int)time(0));
	for (int i = 0; i < deck->size; i++) {
		int j = rand() % deck->size;
		CARD* temp = deck->cards[i];
		deck->cards[i] = deck->cards[j];
		deck->cards[j] = temp;
	}
}

void DeckDestroy(DECK* deck)
{
	for (int i = 0; i < deck->size; i++)
		free(deck->cards[i]);
	free(deck->cards);
	free(deck);
}

//-------------------------------------------------------------------------------------------

PLAYER* PlayerCreate(const char* name, int seed)
{
	srand((int)time(0) + seed);
	int pos = ((rand() + seed) % 15);

	PLAYER* pl = (PLAYER*)malloc(sizeof(PLAYER));
	if (!pl) exit(EXIT_FAILURE);

	if (name != NULL)
		pl->name = name;
	else
	{
		pl->name = names_str[pos];
	}
	pl->hand = NULL;

	return pl;
}

void PlayerKill(PLAYER* player)
{
	for (int i = 0; i < player->hand->size; i++)
		free(player->hand->cards[i]);
	free(player->hand->cards);
	free(player->hand);
	free(player);
}

//-------------------------------------------------------------------------------------------

void HandCreate(PLAYER* player, GAME game_id)
{
	if (!player->hand) free(player->hand);
	int cards_num = 0;

	switch (game_id)
	{
	case EMPTY:
		return;
	case TEXAS:
		cards_num = 2;  // Obviously
		break;
	case FOOL:
		cards_num = 36; // If you are a very unlucky person
		break;
	case TWENTY_ONE:
		cards_num = 4;  // 3*(6) => 18 (score) + any another one
		break;
	case BLACKJACK:
		cards_num = 9;  // 4*(2) + 4*(3) => 20 (score) + any another one
		break;
	default:
		exit(EXIT_FAILURE);
	}

	player->hand = (DECK*)malloc(sizeof(DECK));
	if (!player->hand) exit(EXIT_FAILURE);

	player->hand->size = 0;
	player->hand->cards = (CARD**)calloc(cards_num, sizeof(CARD*));

}

void CardTake(PLAYER* player, DECK* deck)
{
	player->hand->cards[(player->hand->size)++] = DeckPop(deck);
}

//-------------------------------------------------------------------------------------------

TABLE* TableCreate(int table_id)
{
	TABLE* table = (TABLE*)malloc(sizeof(TABLE));
	if (!table) exit(EXIT_FAILURE);

	table-> players_count = 0;
	table->      table_id = table_id;
	table->       game_id = EMPTY;
	table->          deck = NULL;
	table->       players = (PLAYER**)calloc(9, sizeof(PLAYER*));

	return table;
}

void TableSet(TABLE* table, GAME game_id)
{
	table->players_count = 0;
	table->game_id = game_id;
	table->deck = DeckCreate(game_id);
	DeckShuffle(table->deck);

	// Create and append players
	for (int i = 0; i < 9; i++)
		table->players[table->players_count++] = PlayerCreate(NULL, table->table_id + table->players_count);

	// Create hand for each player
	for (int i = 0; i < table->players_count; i++)
		HandCreate(table->players[i], game_id);
}

int TablePlayerAppend(TABLE* table, PLAYER* player)
{
	if (table->players_count == 9) return 1;
	table->players[table->players_count] = player;
	HandCreate(player, table->game_id);
	table->players_count++;
	return 0;
}

int TablePlayerKick(TABLE* table)
{
	if (table->players_count == 0) return 1;
	PlayerKill(table->players[table->players_count - 1]);
	table->players[table->players_count - 1] = NULL;
	table->players_count--;
	return 0;
}

void TableDestroy(TABLE* table)
{
	DeckDestroy(table->deck);
	free(table->players);
	free(table);
}
