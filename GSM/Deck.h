#pragma once
#include "Card.h"
#include <vector>

class Deck
{
public:
	vector<Card> cardsInPlay;
	Deck(void){}
	~Deck(void){}

	void addActive(Card inCard)
	{
		cardsInPlay.push_back(inCard);
	}

	bool isNotActive(Card inCard)
	{
		bool notActive = true;
		for(int i=0;i<cardsInPlay.size();i++)
		{
			if(cardsInPlay[i] == inCard) notActive = false;
		}
		return notActive;
	}

	void resetDeck()
	{
		cardsInPlay.clear();
	}
};

