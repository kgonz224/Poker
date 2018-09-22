/********************************************************************************
|
|	Source code:  cardPlayers.c
|	Author:  Kevin Gonzalez
|	Student ID:  5702392
|	Assignment:  Program #4 - Stud Poker
|
|	Course:  COP 4338 (Programming III)
|	Section:  U01
|	Instructor:  William Feild
|	Due Date:  March 20 2018, class time
|
|	I hereby certify that this collective work is my own
|	and none of it is the work of any other person or entity.
|	______Kevin_Gonzalez_______________ [Signature]
|
|	Language:  C
|
|	Description: 	This source file includes functions used to work with
|			players of a game.
|
*******************************************************************************/

#include "pokerGame.h"

// Used only in this source file
#define THREE_PLUS_RANK_DETERMINANT_POS 2
#define TIE -1
int tieBreaker(const Player, const Player, const int);

// Plus one for no poker rank
const char *pokerRank[NUM_OF_POKER_RANKS + 1] = {"High Card", "One Pair",
		"Two Pairs", "Three of a Kind", "Straight", "Flush",
		"Full House","Four of a Kind", "Straight Flush", "Royal Flush",
		"NR"};

const char *playerStatus[NUM_STATUS] = {"WINNER", "DRAW", "NS"};

/**************************determinePokerRank***********************************
|
|	Function:	determinePokerRank(Player[], const int, const int)
|
|	Purpose:	Determines the Poker Rank of each player from a hand
|				ordered by rank.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@param:		Player players[]
|				Player array of size maxPlayers with hands
|				ordered by rank.
|
|	@param:		const int maxCards
|				Cards per hand.
|
|	@param:		cosnt int numPlayers
|				Number of Players in game.
|
|	@return:	void
|
*******************************************************************************/

void determinePokerRank(Player players[], const int maxCards,
						const int numPlayers)
{
	enum Boolean ace = FALSE;
	enum Boolean flush = TRUE;
	enum Boolean straight = TRUE;
	int repetitionCount = 0;
	int repetitionCount2 = 0;
	int player = EMPTY_INDEX;
	int cardCount = EMPTY_INDEX;

	for(player = 0; player < numPlayers; player++)
	{
		repetitionCount = 0;
		repetitionCount2 = 0;
		ace = FALSE;
		flush = TRUE;
		straight = TRUE;

		for(cardCount = 0; cardCount < maxCards - 1; cardCount++)
		{
			if (cardCount == 0 &&
				players[player].hand[cardCount].rankID == ACE)
			{
				ace = TRUE;

			} //End of ace determiner

			if (cardCount == 1 && ace == TRUE &&
				players[player].hand[cardCount].rankID == TEN)
			{
				straight = TRUE;

			} // End of straight second chance

			if (flush == TRUE &&
				players[player].hand[cardCount].suitID !=
				players[player].hand[cardCount + 1].suitID)
			{
				flush = FALSE;

			} //End of Flush determiner

			if (straight == TRUE &&
				players[player].hand[cardCount].rankID !=
				((players[player].hand[cardCount + 1].rankID) -
				1))
			{
				straight = FALSE;

			} // End of Straight determiner

			// Found adjacent cards of the same rank
			if (players[player].hand[cardCount].rankID ==
				players[player].hand[cardCount + 1].rankID)
			{

				repetitionCount2++;

			}

			else if (repetitionCount == 0)
			{

				repetitionCount = repetitionCount2;
				repetitionCount2 = 0;

			} // End of hand processing

		} // End of cardCount for loop

		// Poker Rank final determinant
		if (straight && flush)
		{
			if (players[player].hand[1].rankID == TEN && ace)
			{
				players[player].pokerRank = ROYAL_FLUSH;
			}

			else
			{
				players[player].pokerRank = STRAIGHT_FLUSH;
			}

		}

		// Not magic. Based of repititions of cards Ex: if FULL HOUSE
		// then one card repeats twice and another repeats once.
		// [2] repeats once [3] repeats twice --> [2] [2] | [3] [3][3]

		else if (repetitionCount == 3 || repetitionCount2 == 3)
		{
			players[player].pokerRank = FOUR_OF_A_KIND;

		}
		else if ((repetitionCount == 2 && repetitionCount2 == 1) ||
				(repetitionCount2 == 2 && repetitionCount == 1))
		{
			players[player].pokerRank = FULL_HOUSE;

		}
		else if (flush)
		{
			players[player].pokerRank = FLUSH;

		}
		else if (straight)
		{
			players[player].pokerRank = STRAIGHT;

		}
		else if (repetitionCount == 2 || repetitionCount2 == 2)
		{
			players[player].pokerRank = THREE_OF_A_KIND;

		}
		else if (repetitionCount == 1 && repetitionCount2 == 1)
		{
			players[player].pokerRank = TWO_PAIRS;

		}
		else if (repetitionCount == 1 || repetitionCount2 == 1)
		{
			players[player].pokerRank = ONE_PAIR;

		}
		else
		{
			players[player].pokerRank = HIGH_CARD;

		} // End of Poker Rank final determinant

	} // End of player for loop

	return;

}

/**************************determinePokerWinner********************************
|
|	Function:	determinePokerWinner(Player[], const int)
|
|	Purpose:	Determines the winner of a game.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@param:		Player players[]
|				Player array of size maxPlayers with hands
|				ordered by rank and containing poker rank.
|
|	@param:		cosnt int numCards
|				Number of cards per players.
|
|	@param:		cosnt int numPlayers
|				Number of players in game.
|
|	@return:	void
|
*******************************************************************************/

void determinePokerWinner(Player players[], const int numCards,
							const int numPlayers)
{
	int player = 0;
	int index = EMPTY_INDEX;
	int lastWinnerIndex = 0;
	int tieWinner = EMPTY_INDEX;
	int winners[numPlayers + 1]; //initialized in first for loop
	enum PokerRank winningRank = players[player].pokerRank;
	enum PlayerStatus status = WINNER;

	winners[0] = player;
	for (index = 1; index <= numPlayers; index++)
	{
		winners[index] = EMPTY_INDEX;
	}

	for (player = 1; player < numPlayers; player++)
	{
		if (players[player].pokerRank == winningRank)
		{
			tieWinner = tieBreaker(players[lastWinnerIndex],
						players[player], numCards);

			if (tieWinner == players[player].playerID)
			{
				index = 0;
				winners[index] = player;
				winners[index + 1] = EMPTY_INDEX;
				lastWinnerIndex = player;
				tieWinner = EMPTY_INDEX;
				status = WINNER;
			}

			else if (tieWinner == TIE)
			{
				index++;
				winners[index] = player;
				winners[index + 1] = EMPTY_INDEX;
				tieWinner = EMPTY_INDEX;
				status = DRAW;
			}

		}

		else if (players[player].pokerRank > winningRank)
		{
			index = 0;
			winners[index] = player;
			winners[index + 1] = EMPTY_INDEX;
			winningRank = players[player].pokerRank;
			lastWinnerIndex = player;
			status = WINNER;
		}
	}

	index = 0;
	while (winners[index] != EMPTY_INDEX)
	{
		players[winners[index]].status = status;
		index++;
	}

	return;
}

/**************************displayPlayersInfo***********************************
|
|	Function:	displayPlayersInfo(const int*, const int, const int);
|
|	Purpose:	Displays each player, their cards, poker rank, and if
|			they are a winner. Uses displayCard()
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@param:		const Player pokerPlayer[0]
|				pokerPlayer is an array of Players
|
|	@param:		const int cardsPerPlayer
|				The number cards per player.
|
|	@param:		const int numOfPlayers
|				The number of players playing.
|
|	@return:	void
|
*******************************************************************************/

void displayPlayersInfo(const Player pokerPlayer[0],
			const int cardsPerPlayer, const int numOfPlayers)
{
	int player = EMPTY_INDEX;
	int cardCount = EMPTY_INDEX;
	for(player = 0; player < numOfPlayers; player++)
	{
		printf("Player %-3d: ", pokerPlayer[player].playerID);

		//Condition in for loop also ensures that card isn't no card.

		for(cardCount = 0; (cardCount < cardsPerPlayer) &&
			(pokerPlayer[player].hand[cardCount].rankID
			!= NO_CARD_R); cardCount++)
		{ //beginning of for

			displayCard(pokerPlayer[player].hand[cardCount]);

		}//end of for

		if (pokerPlayer[player].pokerRank != UNKNOWN_POKER_RANK)
		{
			printf(" - %-15s",
				pokerRank[pokerPlayer[player].pokerRank]);

		} // End of poker rank printing

		if (pokerPlayer[player].status != NO_STATUS)
		{
			printf(" : %s",
				playerStatus[pokerPlayer[player].status]);
		}

		puts("");
	}

	puts("");

	return;
}

/**************************initializePlayers************************************
|
|	Function:	initializePlayersHands()
|
|	Purpose:	Initializes every player with default settings.
|			(Player ID, No card, No rank, Not a winner)
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@param:		Player players[]
|				Player array of size maxPlayers.
|
|	@param:		const int maxPlayers
|				Max players in game.
|
|	@param:		cosnt int maxCards
|				Max cards per hand.
|
|	@return:	void
|
*******************************************************************************/

void initializePlayers(Player players[], const int maxPlayers)
{
	int player = EMPTY_INDEX;
	int cardCount = EMPTY_INDEX;

	for(player = 0; player < maxPlayers; player++)
	{

		players[player].playerID = player + 1;
		for(cardCount = 0; cardCount < MAX_CARDS_PER_HAND; cardCount++)
		{
			players[player].hand[cardCount].rankID = NO_CARD_R;
			players[player].hand[cardCount].suitID = NO_CARD_S;
		}

		players[player].pokerRank = UNKNOWN_POKER_RANK;
		players[player].status = NO_STATUS;

	} //end of player for loop

	return;
}


/******************************orderHandsByRank**********************************
|
|	Function:	orderHandsByRank(players[], const int cardsPerPlayer,
|							const int numPlayers)
|
|	Purpose:	Orders the players' hands in ascending rank order.
|			Ace will be in the front. Uses a bubble sort.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@param:		players[]
|				The array of all the players in the game.
|
|	@param:		const int cardsPerPlayer
|				The number cards per player.
|
|	@param:		const int numOfPlayers
|				The number of players playing.
|
********************************************************************************/

void orderHandsByRank(Player players[], const int cardsPerPlayer,
							const int numPlayers)
{
	int player = EMPTY_INDEX;
	int indexesFixed = EMPTY_INDEX;
	int index = EMPTY_INDEX;
	Card temp = {NO_CARD_R, NO_CARD_S};

	for (player = 0; player < numPlayers; player++)
	{
		for (indexesFixed = 0; indexesFixed < cardsPerPlayer - 1;
								indexesFixed++)
		{
			for (index = 0; index < (cardsPerPlayer - indexesFixed
								- 1); index++)
			{
				if (players[player].hand[index].rankID >
					players[player].hand[index + 1].rankID)
				{
					temp = players[player].hand[index];

					players[player].hand[index] =
						players[player].hand[index + 1];

					players[player].hand[index + 1] = temp;

				}

			} //end of compare loop

		} //end of Card count for loop

	} //end of Player for loop

	return;
}

/************************ *passOutStartingHands**********************************
|
|	Function:	*passOutStartingHands(const int cardsPerPlayer,
|							const int numOfPlayers)
|
|	Purpose:	Passes cards to each player.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@param:		const int cardsPerPlayer
|				The number cards per player.
|
|	@param:		const int numOfPlayers
|				The number of players playing.
|
********************************************************************************/

void passOutStartingHands(Player players[], const int cardsPerPlayer,
								int numOfPlayers)
{
	int player = EMPTY_INDEX;
	int cardCount = EMPTY_INDEX;
	initializePlayers(players, numOfPlayers);

	for(cardCount = 0; cardCount < cardsPerPlayer; cardCount++)
	{
		for(player = 0; player < numOfPlayers; player++)
		{
			players[player].hand[cardCount] = getTopCard();
		}

	} //end of cardCount for loop

	return;
}

/********************************tieBreaker************************************
|
|	Function:	tieBreaker(const Player, const Player, const int)
|
|	Purpose:	Determines the winner between two players with the
|			same poker rank
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@param:		Player player1
|				Player one
|
|	@param:		Player player2
|				Player two
|
|	@param:		int numCards
|				Number of cards in each players hands
|
|	@return:	int
|				returns the PlayerID of winner or returns TIE
|				if theres no winner.
|
*******************************************************************************/

int tieBreaker (const Player player1, const Player player2, const int numCards)
{
	int winner = TIE;
	int ace1 = FALSE;
	int ace2 = FALSE;
	int aceIndex = EMPTY_INDEX;
	int index = EMPTY_INDEX;

	if (player1.hand[0].rankID == ACE)
	{
		ace1 = TRUE;
	}

	if (player2.hand[0].rankID == ACE)
	{
		ace2 = TRUE;
	}

	if (player1.pokerRank == HIGH_CARD ||
		player1.pokerRank == FLUSH)
	{
		aceIndex = 0;
		while (winner == TIE && (player1.hand[aceIndex].rankID == ACE ||
					player2.hand[aceIndex].rankID == ACE))
		{
			if (player1.hand[aceIndex].rankID == ACE &&
					player2.hand[aceIndex].rankID != ACE)
			{
				winner = player1.playerID;
			}

			else if (player1.hand[aceIndex].rankID != ACE &&
					player2.hand[aceIndex].rankID == ACE)
			{
				winner = player2.playerID;
			}

			aceIndex++;

		} // End of ace exeption

		if (winner == TIE)
		{
			for (index = numCards - 1; index >= 0 && winner == TIE;
									index--)
			{
				if (player1.hand[index].rankID >
						player2.hand[index].rankID)
				{

					winner = player1.playerID;
				}

				else if (player1.hand[index].rankID <
						player2.hand[index].rankID)
				{

					winner = player2.playerID;
				}
			}
		}

	} // End of high card and flush tie breaker

	else if (player1.pokerRank == ONE_PAIR ||
		player1.pokerRank == TWO_PAIRS)
	{
		enum Rank pair1Player1 = NO_CARD_R;
		enum Rank pair2Player1 = NO_CARD_R;
		enum Rank pair1Player2 = NO_CARD_R;
		enum Rank pair2Player2 = NO_CARD_R;

		int index2 = EMPTY_INDEX;
		enum Boolean pair = FALSE;
		enum Boolean acePair1 = FALSE;
		enum Boolean acePair2 = FALSE;

		// Find ace pairs
		if (ace1 == TRUE && player1.hand[1].rankID == ACE)
		{

			acePair1 = TRUE;
		}

		if (ace2 == TRUE && player2.hand[1].rankID == ACE)
		{

			acePair2 = TRUE;
		}

		// Find pairs
		for (index = 0; index < numCards - 1; index++)
		{
			if(player1.hand[index].rankID ==
						player1.hand[index + 1].rankID)
			{
				if (pair1Player1 == NO_CARD_R)
				{
					pair1Player1 =
						player1.hand[index].rankID;
				}
				else
				{
					pair2Player1 =
						player1.hand[index].rankID;
				}

				index++;
			}

		} // End of player 1 pair finder

		for (index2 = 0; index2 < numCards - 1; index2++)
		{

			if(player2.hand[index2].rankID ==
						player2.hand[index2 + 1].rankID)
			{

				if (pair1Player2 == NO_CARD_R)
				{
					pair1Player2 =
						player2.hand[index2].rankID;
				}
				else
				{
					pair2Player2 =
						player2.hand[index2].rankID;
				}

				index2++;
			}

		} // End of player 2 pair finder


		// Testing winner of pair 2 if exists
		if (pair2Player1 != NO_CARD_R)
		{
			if (acePair1 > acePair2)
			{

				winner = player1.playerID;
			}

			else if (acePair1 < acePair2)
			{

				winner = player2.playerID;
			}

			else if (pair2Player1 > pair2Player2)
			{

				winner = player1.playerID;
			}

			else if (pair2Player1 < pair2Player2)
			{

				winner = player2.playerID;
			}
		}

		// Testing winner of first pair to occur
		if (winner == TIE)
		{
			if (acePair1 > acePair2)
			{

				winner = player1.playerID;
			}

			else if (acePair1 < acePair2)
			{

				winner = player2.playerID;
			}

			else if (pair1Player1 > pair1Player2)
			{

				winner = player1.playerID;
			}

			else if (pair1Player1 < pair1Player2)
			{

				winner = player2.playerID;
			}
		}

		// Tests for High Card if pairs failed
		if (winner == TIE)
		{
			// no need for check on index two because
			// both players have the same poker rank

			for (index = numCards - 1, index2 = numCards - 1;
				index >= 0 && index2 >= 0 &&  winner == TIE;
				index--, index2--)
			{

				// Skip pairs

				while (index >= 0 && (player1.hand[index].rankID
					== pair1Player1 || player1.hand[index].
					rankID == pair2Player1))
				{

					index--;
				}

				while (index2 >= 0 && (player2.hand[index2].
					rankID == pair1Player2 || player2.
					hand[index2].rankID == pair2Player2))
				{

					index2--;
				}

				// End of skipping pairs

				if (index >= 0 && index2 >= 0)
				{
					if (ace1 != ace2)
					{
						if (ace1 == TRUE)
						{
							winner = player1.
								playerID;
						}

						else
						{
							winner = player2.
								playerID;
						}

					} // End of ace exeption

					else if (player1.hand[index].rankID >
						player2.hand[index2].rankID)
					{

						winner = player1.playerID;
					}

					else if (player1.hand[index].rankID <
						player2.hand[index2].rankID)
					{

						winner = player2.playerID;
					}

				} // End of high card comparison

			} // End of for loop

		} // End of high card comparison for pairs

	} // End of pair tie breaker

	else if (player1.pokerRank == STRAIGHT ||
		player1.pokerRank == STRAIGHT_FLUSH)
	{
		if ((player1.hand[1].rankID == TEN && ace1) &&
				!(player2.hand[1].rankID == TEN && ace2))
		{
			winner = player1.playerID;
		}

		else if (!(player1.hand[1].rankID == TEN && ace1) &&
					(player2.hand[1].rankID == TEN && ace2))
		{
			winner = player2.playerID;
		}
		// End of High Ace exeption.

		else if (player1.hand[numCards - 1].rankID >
					player2.hand[numCards - 1].rankID)
		{
			winner = player1.playerID;
		}

		else if (player1.hand[numCards - 1].rankID <
					player2.hand[numCards - 1].rankID)
		{
			winner = player2.playerID;
		}

	} // End of Straight, straight flush tie breaker

	else
	{
		if (ace1 != ace2)
		{
			if (ace1 == TRUE)
			{
				winner = player1.playerID;
			}

			else
			{
				winner = player2.playerID;
			}

		} // End of ace exeption

		else if (player1.hand[THREE_PLUS_RANK_DETERMINANT_POS].rankID >
			player2.hand[THREE_PLUS_RANK_DETERMINANT_POS].rankID)
		{
			winner = player1.playerID;
		}

		else if (player1.hand[THREE_PLUS_RANK_DETERMINANT_POS].rankID <
			player2.hand[THREE_PLUS_RANK_DETERMINANT_POS].rankID)
		{
			winner = player2.playerID;
		}

	} // End of three of a kind, four of a kind, and full house


	return winner;
}
