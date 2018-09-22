
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#define MAX_PLAYERS 7
#define MIN_PLAYERS 1

#define MAX_STARTING_HAND 5
#define MIN_STARTING_HAND 5

#define MAX_CARDS_PER_HAND 5
#define MAX_CARDS_PER_LINE 4

#define FULL_DECK_SIZE 52
#define NUM_OF_RANKS 14
#define NUM_OF_SUITS 4
#define EMPTY_INDEX -1

#define NUM_STATUS 3

#define NUM_OF_POKER_RANKS 10

enum Boolean {FALSE, TRUE};

enum Suit {CLUB, DIAMOND, SPADE, HEART, NO_CARD_S};
enum Rank {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
						JACK, QUEEN, KING, NO_CARD_R};

enum PokerRank {HIGH_CARD, ONE_PAIR, TWO_PAIRS, THREE_OF_A_KIND, STRAIGHT,
		FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH,
		UNKNOWN_POKER_RANK};

enum PlayerStatus {WINNER, DRAW, NO_STATUS};

typedef struct
{
	enum Rank rankID;
	enum Suit suitID;

} Card;

typedef struct
{
	int playerID;
	Card hand[MAX_CARDS_PER_HAND];
	enum PokerRank pokerRank;
	enum PlayerStatus status;

} Player;

//Plus one for no card symbol.
const char *suit[NUM_OF_SUITS + 1];
const char *rank[NUM_OF_RANKS + 1];
const char *pokerRank[NUM_OF_POKER_RANKS + 1];
const char *playerStatus[NUM_STATUS];

//found in 'cardDeck.c'
void displayCard(Card);
void displayDeck(const Card[]);
Card getTopCard();
Card *grabNewDeck(void);
void shuffleDeck(Card[]);

//Found in 'cardPlayer.c
void determinePokerRank(Player*, const int, const int);
void determinePokerWinner(Player*, const int, const int);
void displayPlayersInfo(const Player*, const int, const int);
void initializePlayers(Player*, const int);
void orderHandsByRank(Player*, const int, const int);
void passOutStartingHands(Player*, const int, const int);
