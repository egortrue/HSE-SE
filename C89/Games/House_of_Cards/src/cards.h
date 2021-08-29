#ifndef CARDS_H

	#define  CARDS_H
	// This header contains some enumirations, structures and functions
	// which are connected to structures manipulation

	typedef enum GAME
	{
		EMPTY,      // Waiting for begin some game
		CLOSE,      // Table is closed
		TEXAS,      // 52 cards | https://en.wikipedia.org/wiki/Texas_hold_%27em
		FOOL,	    // 36 cards | https://en.wikipedia.org/wiki/Durak
		TWENTY_ONE, // 36 cards | https://en.wikipedia.org/wiki/Twenty-One_(card_game)
		BLACKJACK   // 52 cards | https://en.wikipedia.org/wiki/Blackjack
		 
		// The (my) differences between Twenty-One and Blacjack
		// is number of cards and ranks. That's all.
	}GAME;

	typedef enum RANK
	{
		// Default ranks
		TWO, THREE, FOUR, FIVE,
		SIX, SEVEN, EIGHT, NINE, TEN,
		JACK, QUEEN, KING, ACE,

		// Extra ranks / One of ways to scale program
		JOKER, BANNER
	}RANK;

	typedef enum SUIT
	{
		HEARTS, DIAMONDS, CLUBS, SPADES
	}SUIT;

	// Byte field is the greatest thing which ever exist:
	// My sizeof(CARD) is 1 byte == (1/4)sizeof(int)
	typedef struct card
	{
		unsigned char rank : 4; // 2^4 = 16, because of existence 13 faces on default
		unsigned char suit : 2; // 2^2 = 4 , because of existence 4 types on default
	}CARD;

	typedef struct deck
	{
		CARD** cards;
	   short   size;
	}DECK;

	typedef struct player
	{
		const char* name;
		      DECK* hand;
	}PLAYER;

	typedef struct table
	{
		   int    table_id;
		  GAME    game_id;
		  DECK*   deck;
		PLAYER**  players;
		 short    players_count;
	}TABLE;


	  DECK*  DeckCreate        (GAME game_id);
	  CARD*  DeckPop           (DECK* deck);
	  void   DeckShuffle       (DECK* deck);
	  void   DeckDestroy       (DECK* deck);

	PLAYER*  PlayerCreate      (const char* name, int seed);
	  void   PlayerKill        (PLAYER* player);

	  void   HandCreate        (PLAYER* player, GAME game_id);
	  void   CardTake          (PLAYER* player, DECK* deck);

	 TABLE*  TableCreate       (int table_id);
	  void   TableSet          (TABLE* table, GAME game_id);
	   int   TablePlayerAppend (TABLE* table, PLAYER* player);
	   int   TablePlayerKick   (TABLE* table);
	  void   TableDestroy      (TABLE* table);


#endif
