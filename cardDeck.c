/********************************************************************************
|
|	Source code:  cardDeck.c
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
|
|	Description: 	This source file includes functions used to maintain
|			and work with a deck of cards.
|
*******************************************************************************/

#include "pokerGame.h"

int topOfDeckIndex = EMPTY_INDEX;
Card topCard = {NO_CARD_R, NO_CARD_S};

Card deck[FULL_DECK_SIZE];	//Initialized when a new deck is grabed.

const char *suit[NUM_OF_SUITS + 1] = {"\u2663", "\u2666", "\u2660", "\u2665",
									"NC"};

const char *rank[NUM_OF_RANKS + 1] = {"A", "2", "3", "4", "5", "6", "7", "8",
						"9", "T", "J", "Q", "K", "NC"};

/*****************************displayCard**************************************
|
|	Function:	displayCard(int);
|
|	Purpose:	Gets a card and displays it.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@param:		Card card
|				Card to display.
|
|	@return:	void
|
*******************************************************************************/

void displayCard(Card card)
{
	printf("[ %s-%s ]", rank[card.rankID], suit[card.suitID]);
	return;
}

/*******************************displayDeck*************************************
|
|	Function:	displayDeck(const int[]);
|
|	Purpose:	Gets a pointer to a deck (deck[0]) and displays all
|			cards in the deck from top of the deck to end of the
|			deck.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@param:		Card deck[]
|				Card array representing card deck.
|
|	@return:	void
|
*******************************************************************************/

void displayDeck(const Card deck[])
{
	int index = EMPTY_INDEX;
	int cardCounter = 0;

	for(index = topOfDeckIndex; index >= 0; index--)
	{
		displayCard(deck[index]);
		cardCounter++;

		if(index == 0)
		{
			puts("\n");
		}

		else if(cardCounter % MAX_CARDS_PER_LINE == 0)
		{
			puts("");
		}


	} //end of for loop

	return;
}

/*****************************getTopCard****************************************
|
|	Function:	getTopCard(int[])
|
|	Purpose:	Draws the first card of the deck. Returns a NO_CARD
|			if deck is empty and moves top of deck pointer at every
|			draw.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@return:	Card topCard
|				Returns top card of the deck.
|
*******************************************************************************/

Card getTopCard()
{

	Card noCard = {NO_CARD_R, NO_CARD_S};

	if(topOfDeckIndex < 0)
	{
		return noCard;
	}

	topCard = deck[topOfDeckIndex];
	deck[topOfDeckIndex].rankID = NO_CARD_R;
	deck[topOfDeckIndex].suitID = NO_CARD_S;
	topOfDeckIndex--;
	return topCard;
}

/***************************** *grabNewDeck***********************************
|
|	Function:	*grabNewDeck()
|
|	Purpose:	Populates deck with the cards in order of rank.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20th, 2018
|
|	@return:	Card deck
|				Returns pointer to deck of cards.
|
*******************************************************************************/

Card *grabNewDeck()
{
	int index = EMPTY_INDEX;

	for(index = 0; index < FULL_DECK_SIZE; index++)
	{
		deck[index].rankID = index / NUM_OF_SUITS;
		deck[index].suitID = index % NUM_OF_SUITS;
	}

	topOfDeckIndex = FULL_DECK_SIZE - 1;

	return deck;
}

/*****************************shuffleDeck*************************************
|
|	Function:	shuffleDeck()
|
|	Purpose:	Shuffles deck array in place.
|
|	@author:	Kevin Gonzalez
|	@date:		March 20, 2018
|
|	@resouce:	https://en.wikipedia.org/wiki/Fisher-Yates_shuffle#
|				The_modern_algorithm
|
|	@param:		Card deck[]
|				Deck being shuffled.
|
|	@return:	void
|
*******************************************************************************/

void shuffleDeck(Card deck[])
{
	Card temp = {NO_CARD_R, NO_CARD_S};
	int currentIndex = EMPTY_INDEX;
	int randIndex = EMPTY_INDEX;

	for(currentIndex = 0; currentIndex <= topOfDeckIndex; currentIndex++)
	{
		randIndex = rand() % (topOfDeckIndex + 1);
		temp = deck[randIndex];
		deck[randIndex] = deck[currentIndex];
		deck[currentIndex] = temp;
	}

	return;
}
