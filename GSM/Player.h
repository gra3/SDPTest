#pragma once
#include "Card.h"
#include "CardDetect.h"
#include "PokerHand.h"
#include "Odds.h"

class Player
{
public:
	Card hand[2];
	CardDetect *cam;
	int playerNumber;
	double chipTotal;
	double currentBet;
	double lastCurrentBet;
	double totalPutIntoPot;
	bool active;
	bool isD;
	bool isBB;
	bool isSB;
	bool isBet;
	bool isStillInRound;
	bool isAllIn;
	bool possibleWinner;
	bool buyInNextRound;
	PokerHand fullHand;
	double totalWon;
	Odds odds;

	Player* nextPlayer;

	Player* findNextActive();
	Player* findNextActiveAndInRound();

	Player();
	~Player(void);
	void initCamera(int camNum);
	void setPlayerNumber(int playerNum);
	int getPlayerNumber();
	bool isActive();
	void setInactive();
	void setActive();
	void buyIn(double ammount);

	bool isDealer();
	void setDealer(bool val);

	bool isBigBlind();
	void setBigBlind(bool val);

	bool isSmallBlind();
	void setSmallBlind(bool val);

	bool isBetting();
	void setBetting(bool val);

	void addWinnings(double potAmmount);

	double getWinnings();

	void getHand();

	bool bothHandCardsDealt();

	int numberOfHandCards();

	bool operator<(const Player &other);
};

