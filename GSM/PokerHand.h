#pragma once
#include <vector>
#include <algorithm>
#include "Card.h"

using namespace std;

class PokerHand
{
public:
	vector<Card> hand;
	unsigned short int rankCountArray[15];
	unsigned short int suitCountArray[4];
	unsigned int handRank;
	vector<Card> bestFive;
	int highCard;
	int kicker;

	PokerHand(void);
	~PokerHand(void);

	void printArrays();
	void calcHandRank();
	bool flushDetected(vector<Card> &handIn);
	bool straightDetected(vector<Card> &handIn);
	void addCard(Card cardIn);
	bool compareRank(Card a, Card b);
	void sortHandByRank(vector<Card> &handIn);
	bool royalFlush(vector<Card> &handIn);
	bool fourOfAKind(vector<Card> &handIn);
	bool fullHouse(vector<Card> &handIn);
	bool threeOfAKind(vector<Card> &handIn);
	bool twoPair(vector<Card> &handIn);
	bool pair(vector<Card> &handIn);
	void printBestHand();

	bool compareStraightFlush(vector<Card> &handIn);
	bool compareFourOfAKind(vector<Card> &handIn);
	bool compareFullHouse(vector<Card> &handIn);
	bool compareFlush(vector<Card> &handIn);
	bool compareStraight(vector<Card> &handIn);
	bool compareThreeOfAKind(vector<Card> &handIn);
	bool compareTwoPair(vector<Card> &handIn);
	bool comparePair(vector<Card> &handIn);

	bool operator==(const PokerHand &other);
	bool operator<(const PokerHand &other);

	void resetPokerHand();
};

