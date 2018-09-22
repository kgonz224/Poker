/********************************************************************************
|
|	Source code:  pokerMain.c
|	Author:  Kevin Gonzalez
|	Student ID:  5702392
|	Assignment:  Program #4 - Stud Poker
|
|	Course:  COP 4338 (Programming III)
|	Section:  U01
|	Instructor:  William Feild
|	Due Date:  March 20th, 2018, class time
|
|	I hereby certify that this collective work is my own
|	and none of it is the work of any other person or entity.
|	______Kevin_Gonzalez_______________ [Signature]
|
|	Language:  C
|	Compile/Run:
| 		gcc pokerMain.c cardDeck.c cardPlayers.c -std=c99 -o poker.out
|			or
|		make buildPoker
|
|		//-std=c99 is required to print extended ASCII characters
|
| 		./poker.out cardsPerHand numPlayers
|			//cardsPerHand is an integer in range
|			//numPlayers is an integer in range
|
|	************************************************************************
|
|	Description: 	This program takes in two arguments (number of players
|			and number of cards per player). It then displays a new
|			deck and shuffles it. Displays the shuffled deck and
|			passes out cards to each player from the top of the deck
|			(one card per player at a time) and displays it. Then
|			the hands are organized by card rank and displayed. Then
|			the poker hand is determined and displayed followed by
|			the word 'winner' next to the winning hand(s). A series
|			of test hands will be shown to demonstrate that the
|			poker hand determinations works for all poker hands.
|
|	Input:		Number of cards per player and the number of players
|			playing
|
|	Output:		The program will display a new deck of cards in order
|			followed by the shuffled deck followed by the cards each
|			player starts with. Then the hands in order of rank
|			followed by the poker hand the hand belongs too. The
|			winner will be determined and shown. Test cases for each
|			poker hand will be shown as well.
|
|	Process:	The program's steps are as follows:
|
|			1.  Validate or correct user input (terminate program if
|				validation is invalid).
|			2.  Display all card of new deck.
|			3.  Shuffle deck
|			4.  Display Shuffled deck
|			5.  Distribute cards to each player
|			6.  Display each players cards
|			7.  Order hand by rank.
|			8.  Display sorted hands
|			9.  Determine poker hands of each hand.
|			10. Display hand with poker rank.
|			11. Determine winner.
|			12. Display hands, poker ranks, and winner
|			13. Determine ranks of test hands
|			14. Display test hands
|			15. Show tie breaker proof for EXTRA CREDIT
|
|	Required
|	Features Not
|	Included:	Include 'pokerGame.h' in program.
|
|	Known Bugs:  None; the program operates correctly.
|
*******************************************************************************/

#include "pokerGame.h"	//function prototypes and constants for card games

// all defines and protocals local to source file

#define ARG_NUM 3
#define NUM_CARDS_PER_PLAYER_ARG 1
#define NUM_PLAYERS_ARG 2

#define TIE_BREAKER 3

#define NO_ERROR 0
#define ERROR 1

enum Boolean incorrectInput(int, char**);
void passOutTestCards(Player*);
void passOutTestCards2(Player*);
void passOutTestCards3(Player*);
void passOutTestCards4(Player*);
void passOutTestCards5(Player*);
void passOutTestCards6(Player*);
void passOutTestCards7(Player*);
void passOutTestCards8(Player*);
void passOutTestCards9(Player*);
void testPoker(void);

int main(int argc, char *argv[])
{
	if(incorrectInput(argc, argv))
	{

		return ERROR;
	}

	//Cards per player is independent of user input.
	const int cardsPerPlayer = MAX_STARTING_HAND;
	const int numPlayers = atoi(argv[NUM_PLAYERS_ARG]);
	Card *deckPtr = NULL;

	//initialized when cards are passed out.
	Player players[MAX_PLAYERS];

	srand(time(NULL));

	puts("New Deck:");
	deckPtr = grabNewDeck();
	displayDeck(deckPtr);

	puts("Shuffled Deck:");
	shuffleDeck(deckPtr);
	displayDeck(deckPtr);

	passOutStartingHands(players, cardsPerPlayer, numPlayers);

	puts("Players Hands:");
	displayPlayersInfo(players, cardsPerPlayer, numPlayers);

	puts("Ordered Player Hands:");
	orderHandsByRank(players, cardsPerPlayer, numPlayers);
	displayPlayersInfo(players, cardsPerPlayer, numPlayers);

	puts("Player Hands with Poker Rank:");
	determinePokerRank(players, cardsPerPlayer, numPlayers);
	displayPlayersInfo(players, cardsPerPlayer, numPlayers);

	puts("Player Hands with Poker Rank and Winner:");
	determinePokerWinner(players, cardsPerPlayer, numPlayers);
	displayPlayersInfo(players, cardsPerPlayer, numPlayers);

	puts("\nTest trials:");
	testPoker();

	return NO_ERROR;
}

/****************************incorrectInput*************************************
|
|	Function:	incorectInput(int, char**);
|
|	Purpose:	Gets command line arguments and determines whether the
|			input is valid for this program. This function will
|			inform users of possible errors.
|
|	@author:	Kevin Gonzalez
|	@date:		February 21, 2018
|
|	@param:		int argc
|				Number of arguments passed through the command
|				line.
|
|	@param:		char **argv
|				Pointer to a character array containing the
|				arguments passed through the command line.
|
|	@return:	enum boolean error
|				Returns TRUE is there is a violation in the
|				input. Returns FALSE if user input is valid.
|
*******************************************************************************/

enum Boolean incorrectInput(int argc, char **argv)
{
	int cardsPerPlayer = -1;
	int numPlayers = -1;
	int cardsRequired = -1;
	enum Boolean error = FALSE;

	if(argc != ARG_NUM)
	{

		printf("Incorrect number of arguments.\n First argument should "
			"be an integer in range [%d-%d] representing the "
			"number of starting cards per player.\n Second argument"
			" should be an interger in range [%d-%d] representing "
			"the number of players participating in the game.\n "
			"(Keep in mind that a full deck contains %d cards)\n\n",
			MIN_STARTING_HAND, MAX_STARTING_HAND, MIN_PLAYERS,
			MAX_PLAYERS, FULL_DECK_SIZE);

		error = TRUE;
	}

	else
	{

		//Number of cards per play is independent of user input
		cardsPerPlayer = MAX_STARTING_HAND;
		numPlayers = atoi(argv[NUM_PLAYERS_ARG]);
		cardsRequired = cardsPerPlayer * numPlayers;

		if((MIN_STARTING_HAND > cardsPerPlayer) ||
			(cardsPerPlayer > MAX_STARTING_HAND))
		{

			printf("Incorrect input in the first argument.\n The "
				"first argument should be an integer in range "
				"[%d-%d] representing the number of starting "
				"cards per player.\n\n", MIN_STARTING_HAND,
				MAX_STARTING_HAND, MIN_PLAYERS, MAX_PLAYERS,
				FULL_DECK_SIZE);

			error = TRUE;
		}

		if((MIN_PLAYERS > numPlayers) || (numPlayers > MAX_PLAYERS))
		{

			printf("Incorrect input in the second argument.\n The "
				"second argument should be an integer in range "
				"[%d-%d] representing the number of starting "
				"cards per player.\n\n", MIN_PLAYERS,
				MAX_PLAYERS, FULL_DECK_SIZE);

			error = TRUE;
		}

		if(cardsRequired > FULL_DECK_SIZE)
		{
			printf("Not enough card in deck.\n Remember that the "
				"deck contains %d cards. Please redefine the "
				"number of cards per player and/or the number "
				"of players to use up at most %d cards.\n\n",
				FULL_DECK_SIZE, FULL_DECK_SIZE);

			error = TRUE;
		}

	} //End of else

	return error;
}

/******************************passOutTestCards*********************************
|
|	Function:	passOutTestCards(Player*);
|
|	Purpose:	Gives each player a test hand.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards(Player testPlayers[])
{

	testPlayers[HIGH_CARD].hand[0].rankID = ACE;
	testPlayers[HIGH_CARD].hand[0].suitID = CLUB;
	testPlayers[HIGH_CARD].hand[1].rankID = FIVE;
	testPlayers[HIGH_CARD].hand[1].suitID = HEART;
	testPlayers[HIGH_CARD].hand[2].rankID = TWO;
	testPlayers[HIGH_CARD].hand[2].suitID = CLUB;
	testPlayers[HIGH_CARD].hand[3].rankID = TEN;
	testPlayers[HIGH_CARD].hand[3].suitID = CLUB;
	testPlayers[HIGH_CARD].hand[4].rankID = KING;
	testPlayers[HIGH_CARD].hand[4].suitID = DIAMOND;

	testPlayers[ONE_PAIR].hand[0].rankID = TWO;
	testPlayers[ONE_PAIR].hand[0].suitID = HEART;
	testPlayers[ONE_PAIR].hand[1].rankID = THREE;
	testPlayers[ONE_PAIR].hand[1].suitID = HEART;
	testPlayers[ONE_PAIR].hand[2].rankID = QUEEN;
	testPlayers[ONE_PAIR].hand[2].suitID = HEART;
	testPlayers[ONE_PAIR].hand[3].rankID = SEVEN;
	testPlayers[ONE_PAIR].hand[3].suitID = HEART;
	testPlayers[ONE_PAIR].hand[4].rankID = QUEEN;
	testPlayers[ONE_PAIR].hand[4].suitID = CLUB;

	testPlayers[TWO_PAIRS].hand[0].rankID = THREE;
	testPlayers[TWO_PAIRS].hand[0].suitID = SPADE;
	testPlayers[TWO_PAIRS].hand[1].rankID = THREE;
	testPlayers[TWO_PAIRS].hand[1].suitID = HEART;
	testPlayers[TWO_PAIRS].hand[2].rankID = JACK;
	testPlayers[TWO_PAIRS].hand[2].suitID = SPADE;
	testPlayers[TWO_PAIRS].hand[3].rankID = KING;
	testPlayers[TWO_PAIRS].hand[3].suitID = SPADE;
	testPlayers[TWO_PAIRS].hand[4].rankID = JACK;
	testPlayers[TWO_PAIRS].hand[4].suitID = DIAMOND;

	testPlayers[THREE_OF_A_KIND].hand[0].rankID = SIX;
	testPlayers[THREE_OF_A_KIND].hand[0].suitID = DIAMOND;
	testPlayers[THREE_OF_A_KIND].hand[1].rankID = SEVEN;
	testPlayers[THREE_OF_A_KIND].hand[1].suitID = SPADE;
	testPlayers[THREE_OF_A_KIND].hand[2].rankID = SIX;
	testPlayers[THREE_OF_A_KIND].hand[2].suitID = SPADE;
	testPlayers[THREE_OF_A_KIND].hand[3].rankID = NINE;
	testPlayers[THREE_OF_A_KIND].hand[3].suitID = CLUB;
	testPlayers[THREE_OF_A_KIND].hand[4].rankID = SIX;
	testPlayers[THREE_OF_A_KIND].hand[4].suitID = HEART;

	testPlayers[STRAIGHT].hand[0].rankID = SIX;
	testPlayers[STRAIGHT].hand[0].suitID = CLUB;
	testPlayers[STRAIGHT].hand[1].rankID = SEVEN;
	testPlayers[STRAIGHT].hand[1].suitID = DIAMOND;
	testPlayers[STRAIGHT].hand[2].rankID = EIGHT;
	testPlayers[STRAIGHT].hand[2].suitID = SPADE;
	testPlayers[STRAIGHT].hand[3].rankID = NINE;
	testPlayers[STRAIGHT].hand[3].suitID = DIAMOND;
	testPlayers[STRAIGHT].hand[4].rankID = TEN;
	testPlayers[STRAIGHT].hand[4].suitID = SPADE;

	testPlayers[FLUSH].hand[0].rankID = ACE;
	testPlayers[FLUSH].hand[0].suitID = HEART;
	testPlayers[FLUSH].hand[1].rankID = FOUR;
	testPlayers[FLUSH].hand[1].suitID = HEART;
	testPlayers[FLUSH].hand[2].rankID = EIGHT;
	testPlayers[FLUSH].hand[2].suitID = HEART;
	testPlayers[FLUSH].hand[3].rankID = JACK;
	testPlayers[FLUSH].hand[3].suitID = HEART;
	testPlayers[FLUSH].hand[4].rankID = QUEEN;
	testPlayers[FLUSH].hand[4].suitID = HEART;

	testPlayers[FULL_HOUSE].hand[0].rankID = EIGHT;
	testPlayers[FULL_HOUSE].hand[0].suitID = DIAMOND;
	testPlayers[FULL_HOUSE].hand[1].rankID = FOUR;
	testPlayers[FULL_HOUSE].hand[1].suitID = SPADE;
	testPlayers[FULL_HOUSE].hand[2].rankID = FOUR;
	testPlayers[FULL_HOUSE].hand[2].suitID = CLUB;
	testPlayers[FULL_HOUSE].hand[3].rankID = EIGHT;
	testPlayers[FULL_HOUSE].hand[3].suitID = SPADE;
	testPlayers[FULL_HOUSE].hand[4].rankID = EIGHT;
	testPlayers[FULL_HOUSE].hand[4].suitID = HEART;

	testPlayers[FOUR_OF_A_KIND].hand[0].rankID = THREE;
	testPlayers[FOUR_OF_A_KIND].hand[0].suitID = CLUB;
	testPlayers[FOUR_OF_A_KIND].hand[1].rankID = TEN;
	testPlayers[FOUR_OF_A_KIND].hand[1].suitID = CLUB;
	testPlayers[FOUR_OF_A_KIND].hand[2].rankID = TEN;
	testPlayers[FOUR_OF_A_KIND].hand[2].suitID = DIAMOND;
	testPlayers[FOUR_OF_A_KIND].hand[3].rankID = TEN;
	testPlayers[FOUR_OF_A_KIND].hand[3].suitID = SPADE;
	testPlayers[FOUR_OF_A_KIND].hand[4].rankID = TEN;
	testPlayers[FOUR_OF_A_KIND].hand[4].suitID = HEART;

	testPlayers[STRAIGHT_FLUSH].hand[0].rankID = FOUR;
	testPlayers[STRAIGHT_FLUSH].hand[0].suitID = DIAMOND;
	testPlayers[STRAIGHT_FLUSH].hand[1].rankID = FIVE;
	testPlayers[STRAIGHT_FLUSH].hand[1].suitID = DIAMOND;
	testPlayers[STRAIGHT_FLUSH].hand[2].rankID = SIX;
	testPlayers[STRAIGHT_FLUSH].hand[2].suitID = DIAMOND;
	testPlayers[STRAIGHT_FLUSH].hand[3].rankID = SEVEN;
	testPlayers[STRAIGHT_FLUSH].hand[3].suitID = DIAMOND;
	testPlayers[STRAIGHT_FLUSH].hand[4].rankID = EIGHT;
	testPlayers[STRAIGHT_FLUSH].hand[4].suitID = DIAMOND;

	testPlayers[ROYAL_FLUSH].hand[0].rankID = ACE;
	testPlayers[ROYAL_FLUSH].hand[0].suitID = CLUB;
	testPlayers[ROYAL_FLUSH].hand[1].rankID = TEN;
	testPlayers[ROYAL_FLUSH].hand[1].suitID = CLUB;
	testPlayers[ROYAL_FLUSH].hand[2].rankID = JACK;
	testPlayers[ROYAL_FLUSH].hand[2].suitID = CLUB;
	testPlayers[ROYAL_FLUSH].hand[3].rankID = QUEEN;
	testPlayers[ROYAL_FLUSH].hand[3].suitID = CLUB;
	testPlayers[ROYAL_FLUSH].hand[4].rankID = KING;
	testPlayers[ROYAL_FLUSH].hand[4].suitID = CLUB;

	return;
}

/******************************passOutTestCards2********************************
|
|	Function:	passOutTestCards2(Player*);
|
|	Purpose:	Gives each player a test hand to compare High cards.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards2(Player testPlayers[])
{

	testPlayers[0].hand[0].rankID = ACE;
	testPlayers[0].hand[0].suitID = CLUB;
	testPlayers[0].hand[1].rankID = FIVE;
	testPlayers[0].hand[1].suitID = HEART;
	testPlayers[0].hand[2].rankID = TWO;
	testPlayers[0].hand[2].suitID = CLUB;
	testPlayers[0].hand[3].rankID = TEN;
	testPlayers[0].hand[3].suitID = CLUB;
	testPlayers[0].hand[4].rankID = KING;
	testPlayers[0].hand[4].suitID = DIAMOND;

	testPlayers[1].hand[0].rankID = TWO;
	testPlayers[1].hand[0].suitID = HEART;
	testPlayers[1].hand[1].rankID = THREE;
	testPlayers[1].hand[1].suitID = HEART;
	testPlayers[1].hand[2].rankID = QUEEN;
	testPlayers[1].hand[2].suitID = HEART;
	testPlayers[1].hand[3].rankID = SEVEN;
	testPlayers[1].hand[3].suitID = HEART;
	testPlayers[1].hand[4].rankID = TEN;
	testPlayers[1].hand[4].suitID = CLUB;

	testPlayers[2].hand[0].rankID = ACE;
	testPlayers[2].hand[0].suitID = SPADE;
	testPlayers[2].hand[1].rankID = THREE;
	testPlayers[2].hand[1].suitID = HEART;
	testPlayers[2].hand[2].rankID = JACK;
	testPlayers[2].hand[2].suitID = SPADE;
	testPlayers[2].hand[3].rankID = KING;
	testPlayers[2].hand[3].suitID = SPADE;
	testPlayers[2].hand[4].rankID = TWO;
	testPlayers[2].hand[4].suitID = DIAMOND;

	return;
}

/******************************passOutTestCards3********************************
|
|	Function:	passOutTestCards3(Player*);
|
|	Purpose:	Gives each player a test hand to compare One Pairs.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards3(Player testPlayers[])
{

	testPlayers[0].hand[0].rankID = ACE;
	testPlayers[0].hand[0].suitID = CLUB;
	testPlayers[0].hand[1].rankID = FIVE;
	testPlayers[0].hand[1].suitID = HEART;
	testPlayers[0].hand[2].rankID = TWO;
	testPlayers[0].hand[2].suitID = CLUB;
	testPlayers[0].hand[3].rankID = TEN;
	testPlayers[0].hand[3].suitID = CLUB;
	testPlayers[0].hand[4].rankID = TEN;
	testPlayers[0].hand[4].suitID = DIAMOND;

	testPlayers[1].hand[0].rankID = ACE;
	testPlayers[1].hand[0].suitID = HEART;
	testPlayers[1].hand[1].rankID = THREE;
	testPlayers[1].hand[1].suitID = HEART;
	testPlayers[1].hand[2].rankID = QUEEN;
	testPlayers[1].hand[2].suitID = HEART;
	testPlayers[1].hand[3].rankID = JACK;
	testPlayers[1].hand[3].suitID = HEART;
	testPlayers[1].hand[4].rankID = ACE;
	testPlayers[1].hand[4].suitID = CLUB;

	testPlayers[2].hand[0].rankID = ACE;
	testPlayers[2].hand[0].suitID = SPADE;
	testPlayers[2].hand[1].rankID = TWO;
	testPlayers[2].hand[1].suitID = HEART;
	testPlayers[2].hand[2].rankID = JACK;
	testPlayers[2].hand[2].suitID = SPADE;
	testPlayers[2].hand[3].rankID = QUEEN;
	testPlayers[2].hand[3].suitID = SPADE;
	testPlayers[2].hand[4].rankID = ACE;
	testPlayers[2].hand[4].suitID = DIAMOND;

	return;
}

/******************************passOutTestCards4********************************
|
|	Function:	passOutTestCards4(Player*);
|
|	Purpose:	Gives each player a test hand to compare Two Pairs.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards4(Player testPlayers[])
{

	testPlayers[0].hand[0].rankID = ACE;
	testPlayers[0].hand[0].suitID = CLUB;
	testPlayers[0].hand[1].rankID = KING;
	testPlayers[0].hand[1].suitID = HEART;
	testPlayers[0].hand[2].rankID = KING;
	testPlayers[0].hand[2].suitID = CLUB;
	testPlayers[0].hand[3].rankID = THREE;
	testPlayers[0].hand[3].suitID = CLUB;
	testPlayers[0].hand[4].rankID = ACE;
	testPlayers[0].hand[4].suitID = DIAMOND;

	testPlayers[2].hand[0].rankID = ACE;
	testPlayers[2].hand[0].suitID = HEART;
	testPlayers[2].hand[1].rankID = ACE;
	testPlayers[2].hand[1].suitID = HEART;
	testPlayers[2].hand[2].rankID = KING;
	testPlayers[2].hand[2].suitID = HEART;
	testPlayers[2].hand[3].rankID = KING;
	testPlayers[2].hand[3].suitID = HEART;
	testPlayers[2].hand[4].rankID = TEN;
	testPlayers[2].hand[4].suitID = CLUB;

	testPlayers[1].hand[0].rankID = ACE;
	testPlayers[1].hand[0].suitID = SPADE;
	testPlayers[1].hand[1].rankID = ACE;
	testPlayers[1].hand[1].suitID = HEART;
	testPlayers[1].hand[2].rankID = JACK;
	testPlayers[1].hand[2].suitID = SPADE;
	testPlayers[1].hand[3].rankID = JACK;
	testPlayers[1].hand[3].suitID = SPADE;
	testPlayers[1].hand[4].rankID = FIVE;
	testPlayers[1].hand[4].suitID = DIAMOND;

	return;
}

/******************************passOutTestCards5********************************
|
|	Function:	passOutTestCards5(Player*);
|
|	Purpose:	Gives each player a test hand to compare Three of a Kind
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards5(Player testPlayers[])
{

	testPlayers[0].hand[0].rankID = ACE;
	testPlayers[0].hand[0].suitID = CLUB;
	testPlayers[0].hand[1].rankID = ACE;
	testPlayers[0].hand[1].suitID = HEART;
	testPlayers[0].hand[2].rankID = TEN;
	testPlayers[0].hand[2].suitID = CLUB;
	testPlayers[0].hand[3].rankID = TEN;
	testPlayers[0].hand[3].suitID = CLUB;
	testPlayers[0].hand[4].rankID = TEN;
	testPlayers[0].hand[4].suitID = DIAMOND;

	testPlayers[1].hand[0].rankID = ACE;
	testPlayers[1].hand[0].suitID = HEART;
	testPlayers[1].hand[1].rankID = QUEEN;
	testPlayers[1].hand[1].suitID = HEART;
	testPlayers[1].hand[2].rankID = QUEEN;
	testPlayers[1].hand[2].suitID = HEART;
	testPlayers[1].hand[3].rankID = QUEEN;
	testPlayers[1].hand[3].suitID = HEART;
	testPlayers[1].hand[4].rankID = ACE;
	testPlayers[1].hand[4].suitID = CLUB;

	testPlayers[2].hand[0].rankID = TWO;
	testPlayers[2].hand[0].suitID = SPADE;
	testPlayers[2].hand[1].rankID = TWO;
	testPlayers[2].hand[1].suitID = HEART;
	testPlayers[2].hand[2].rankID = JACK;
	testPlayers[2].hand[2].suitID = SPADE;
	testPlayers[2].hand[3].rankID = JACK;
	testPlayers[2].hand[3].suitID = SPADE;
	testPlayers[2].hand[4].rankID = JACK;
	testPlayers[2].hand[4].suitID = DIAMOND;

	return;
}

/******************************passOutTestCards6********************************
|
|	Function:	passOutTestCards6(Player*);
|
|	Purpose:	Gives each player a test hand to compare Straight.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards6(Player testPlayers[])
{

	testPlayers[0].hand[0].rankID = ACE;
	testPlayers[0].hand[0].suitID = CLUB;
	testPlayers[0].hand[1].rankID = FIVE;
	testPlayers[0].hand[1].suitID = HEART;
	testPlayers[0].hand[2].rankID = FOUR;
	testPlayers[0].hand[2].suitID = CLUB;
	testPlayers[0].hand[3].rankID = THREE;
	testPlayers[0].hand[3].suitID = CLUB;
	testPlayers[0].hand[4].rankID = TWO;
	testPlayers[0].hand[4].suitID = DIAMOND;

	testPlayers[1].hand[0].rankID = FOUR;
	testPlayers[1].hand[0].suitID = HEART;
	testPlayers[1].hand[1].rankID = FIVE;
	testPlayers[1].hand[1].suitID = HEART;
	testPlayers[1].hand[2].rankID = SIX;
	testPlayers[1].hand[2].suitID = HEART;
	testPlayers[1].hand[3].rankID = SEVEN;
	testPlayers[1].hand[3].suitID = HEART;
	testPlayers[1].hand[4].rankID = EIGHT;
	testPlayers[1].hand[4].suitID = CLUB;

	testPlayers[2].hand[0].rankID = FOUR;
	testPlayers[2].hand[0].suitID = SPADE;
	testPlayers[2].hand[1].rankID = FIVE;
	testPlayers[2].hand[1].suitID = HEART;
	testPlayers[2].hand[2].rankID = SIX;
	testPlayers[2].hand[2].suitID = SPADE;
	testPlayers[2].hand[3].rankID = SEVEN;
	testPlayers[2].hand[3].suitID = SPADE;
	testPlayers[2].hand[4].rankID = EIGHT;
	testPlayers[2].hand[4].suitID = DIAMOND;

	return;
}

/******************************passOutTestCards7********************************
|
|	Function:	passOutTestCards7(Player*);
|
|	Purpose:	Gives each player a test hand to compare Flush.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards7(Player testPlayers[])
{

	testPlayers[0].hand[0].rankID = ACE;
	testPlayers[0].hand[0].suitID = CLUB;
	testPlayers[0].hand[1].rankID = JACK;
	testPlayers[0].hand[1].suitID = CLUB;
	testPlayers[0].hand[2].rankID = JACK;
	testPlayers[0].hand[2].suitID = CLUB;
	testPlayers[0].hand[3].rankID = TEN;
	testPlayers[0].hand[3].suitID = CLUB;
	testPlayers[0].hand[4].rankID = ACE;
	testPlayers[0].hand[4].suitID = CLUB;

	testPlayers[1].hand[0].rankID = ACE;
	testPlayers[1].hand[0].suitID = HEART;
	testPlayers[1].hand[1].rankID = THREE;
	testPlayers[1].hand[1].suitID = HEART;
	testPlayers[1].hand[2].rankID = QUEEN;
	testPlayers[1].hand[2].suitID = HEART;
	testPlayers[1].hand[3].rankID = QUEEN;
	testPlayers[1].hand[3].suitID = HEART;
	testPlayers[1].hand[4].rankID = THREE;
	testPlayers[1].hand[4].suitID = HEART;

	testPlayers[2].hand[0].rankID = ACE;
	testPlayers[2].hand[0].suitID = SPADE;
	testPlayers[2].hand[1].rankID = ACE;
	testPlayers[2].hand[1].suitID = SPADE;
	testPlayers[2].hand[2].rankID = JACK;
	testPlayers[2].hand[2].suitID = SPADE;
	testPlayers[2].hand[3].rankID = JACK;
	testPlayers[2].hand[3].suitID = SPADE;
	testPlayers[2].hand[4].rankID = FIVE;
	testPlayers[2].hand[4].suitID = SPADE;

	return;
}

/******************************passOutTestCards8********************************
|
|	Function:	passOutTestCards8(Player*);
|
|	Purpose:	Gives each player a test hand to compare Full House.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards8(Player testPlayers[])
{

	testPlayers[0].hand[0].rankID = ACE;
	testPlayers[0].hand[0].suitID = CLUB;
	testPlayers[0].hand[1].rankID = KING;
	testPlayers[0].hand[1].suitID = HEART;
	testPlayers[0].hand[2].rankID = KING;
	testPlayers[0].hand[2].suitID = CLUB;
	testPlayers[0].hand[3].rankID = ACE;
	testPlayers[0].hand[3].suitID = CLUB;
	testPlayers[0].hand[4].rankID = ACE;
	testPlayers[0].hand[4].suitID = DIAMOND;

	testPlayers[1].hand[0].rankID = QUEEN;
	testPlayers[1].hand[0].suitID = HEART;
	testPlayers[1].hand[1].rankID = THREE;
	testPlayers[1].hand[1].suitID = HEART;
	testPlayers[1].hand[2].rankID = QUEEN;
	testPlayers[1].hand[2].suitID = HEART;
	testPlayers[1].hand[3].rankID = QUEEN;
	testPlayers[1].hand[3].suitID = HEART;
	testPlayers[1].hand[4].rankID = THREE;
	testPlayers[1].hand[4].suitID = CLUB;

	testPlayers[2].hand[0].rankID = SEVEN;
	testPlayers[2].hand[0].suitID = SPADE;
	testPlayers[2].hand[1].rankID = SEVEN;
	testPlayers[2].hand[1].suitID = HEART;
	testPlayers[2].hand[2].rankID = JACK;
	testPlayers[2].hand[2].suitID = SPADE;
	testPlayers[2].hand[3].rankID = JACK;
	testPlayers[2].hand[3].suitID = SPADE;
	testPlayers[2].hand[4].rankID = SEVEN;
	testPlayers[2].hand[4].suitID = DIAMOND;

	return;
}

/******************************passOutTestCards9********************************
|
|	Function:	passOutTestCards9(Player*);
|
|	Purpose:	Gives each player a test hand to compare Four of a Kind.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards9(Player testPlayers[])
{

	testPlayers[0].hand[0].rankID = KING;
	testPlayers[0].hand[0].suitID = CLUB;
	testPlayers[0].hand[1].rankID = KING;
	testPlayers[0].hand[1].suitID = HEART;
	testPlayers[0].hand[2].rankID = KING;
	testPlayers[0].hand[2].suitID = CLUB;
	testPlayers[0].hand[3].rankID = TEN;
	testPlayers[0].hand[3].suitID = CLUB;
	testPlayers[0].hand[4].rankID = KING;
	testPlayers[0].hand[4].suitID = DIAMOND;

	testPlayers[1].hand[0].rankID = QUEEN;
	testPlayers[1].hand[0].suitID = HEART;
	testPlayers[1].hand[1].rankID = THREE;
	testPlayers[1].hand[1].suitID = HEART;
	testPlayers[1].hand[2].rankID = THREE;
	testPlayers[1].hand[2].suitID = HEART;
	testPlayers[1].hand[3].rankID = THREE;
	testPlayers[1].hand[3].suitID = HEART;
	testPlayers[1].hand[4].rankID = THREE;
	testPlayers[1].hand[4].suitID = CLUB;

	testPlayers[2].hand[0].rankID = ACE;
	testPlayers[2].hand[0].suitID = SPADE;
	testPlayers[2].hand[1].rankID = ACE;
	testPlayers[2].hand[1].suitID = HEART;
	testPlayers[2].hand[2].rankID = ACE;
	testPlayers[2].hand[2].suitID = SPADE;
	testPlayers[2].hand[3].rankID = JACK;
	testPlayers[2].hand[3].suitID = SPADE;
	testPlayers[2].hand[4].rankID = ACE;
	testPlayers[2].hand[4].suitID = DIAMOND;

	return;
}

/******************************passOutTestCards10********************************
|
|	Function:	passOutTestCards10(Player*);
|
|	Purpose:	Gives each player a test hand to compare Straight Flush.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Player player[]
|				Player array with the same number of players
|				as test hands
|
*******************************************************************************/

void passOutTestCards10(Player testPlayers[])
{

	testPlayers[0].hand[0].rankID = KING;
	testPlayers[0].hand[0].suitID = CLUB;
	testPlayers[0].hand[1].rankID = QUEEN;
	testPlayers[0].hand[1].suitID = CLUB;
	testPlayers[0].hand[2].rankID = JACK;
	testPlayers[0].hand[2].suitID = CLUB;
	testPlayers[0].hand[3].rankID = TEN;
	testPlayers[0].hand[3].suitID = CLUB;
	testPlayers[0].hand[4].rankID = NINE;
	testPlayers[0].hand[4].suitID = CLUB;

	testPlayers[1].hand[0].rankID = THREE;
	testPlayers[1].hand[0].suitID = HEART;
	testPlayers[1].hand[1].rankID = FOUR;
	testPlayers[1].hand[1].suitID = HEART;
	testPlayers[1].hand[2].rankID = FIVE;
	testPlayers[1].hand[2].suitID = HEART;
	testPlayers[1].hand[3].rankID = SIX;
	testPlayers[1].hand[3].suitID = HEART;
	testPlayers[1].hand[4].rankID = SEVEN;
	testPlayers[1].hand[4].suitID = HEART;

	testPlayers[2].hand[0].rankID = ACE;
	testPlayers[2].hand[0].suitID = SPADE;
	testPlayers[2].hand[1].rankID = TWO;
	testPlayers[2].hand[1].suitID = SPADE;
	testPlayers[2].hand[2].rankID = FIVE;
	testPlayers[2].hand[2].suitID = SPADE;
	testPlayers[2].hand[3].rankID = FOUR;
	testPlayers[2].hand[3].suitID = SPADE;
	testPlayers[2].hand[4].rankID = THREE;
	testPlayers[2].hand[4].suitID = SPADE;

	return;
}

/********************************testPoker**************************************
|
|	Function:	testPoker;
|
|	Purpose:	Creates a test hands with all the poker ranks and uses
|			pokerRankFinder and displayPlayersInfo to check for all
|			cases.
|
|	@author:	Kevin Gonzalez
|	@date:		March 15th, 2018
|
*******************************************************************************/

void testPoker()
{
	puts("Poker Rank Determiner");
	Player testPlayers[NUM_OF_POKER_RANKS];
	initializePlayers(testPlayers, NUM_OF_POKER_RANKS);
	passOutTestCards(testPlayers);
	orderHandsByRank(testPlayers, MAX_STARTING_HAND, NUM_OF_POKER_RANKS);
	determinePokerRank(testPlayers, MAX_STARTING_HAND, NUM_OF_POKER_RANKS);
	determinePokerWinner(testPlayers, MAX_STARTING_HAND, NUM_OF_POKER_RANKS);
	displayPlayersInfo(testPlayers, MAX_STARTING_HAND, NUM_OF_POKER_RANKS);

	puts("High Card Tie Breaker");
	Player testPlayersTB[TIE_BREAKER];
	initializePlayers(testPlayersTB, TIE_BREAKER);
	passOutTestCards2(testPlayersTB);
	orderHandsByRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerWinner(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	displayPlayersInfo(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);

	puts("One Pair Tie Breaker");
	initializePlayers(testPlayersTB, TIE_BREAKER);
	passOutTestCards3(testPlayersTB);
	orderHandsByRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerWinner(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	displayPlayersInfo(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);

	puts("Two Pair Tie Breaker");
	initializePlayers(testPlayersTB, TIE_BREAKER);
	passOutTestCards4(testPlayersTB);
	orderHandsByRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerWinner(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	displayPlayersInfo(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);

	puts("Three of a King Tie Breaker");
	initializePlayers(testPlayersTB, TIE_BREAKER);
	passOutTestCards5(testPlayersTB);
	orderHandsByRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerWinner(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	displayPlayersInfo(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);

	puts("Straight Tie Breaker");
	initializePlayers(testPlayersTB, TIE_BREAKER);
	passOutTestCards6(testPlayersTB);
	orderHandsByRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerWinner(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	displayPlayersInfo(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);

	puts("Flush Tie Breaker");
	initializePlayers(testPlayersTB, TIE_BREAKER);
	passOutTestCards7(testPlayersTB);
	orderHandsByRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerWinner(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	displayPlayersInfo(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);

	puts("Full House Tie Breaker");
	initializePlayers(testPlayersTB, TIE_BREAKER);
	passOutTestCards8(testPlayersTB);
	orderHandsByRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerWinner(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	displayPlayersInfo(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);

	puts("Four of a Kind Tie Breaker");
	initializePlayers(testPlayersTB, TIE_BREAKER);
	passOutTestCards9(testPlayersTB);
	orderHandsByRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerWinner(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	displayPlayersInfo(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);

	puts("Straight Flush Tie Breaker");
	initializePlayers(testPlayersTB, TIE_BREAKER);
	passOutTestCards10(testPlayersTB);
	orderHandsByRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerRank(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	determinePokerWinner(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);
	displayPlayersInfo(testPlayersTB, MAX_STARTING_HAND, TIE_BREAKER);

	return;
}

