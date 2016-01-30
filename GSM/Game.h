#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip> 
#include "Card.h"
#include "Player.h"
#include "Deck.h"
#include "PokerHand.h"

enum s {START,BLINDS,DEALING,BETTING,COMMCARD,HANDRES,CLEANUP};

class Game
{
public:
	s state;
	Deck deck;
	Card commCard[5];
	vector<Player> player;
	int numberOfPlayers;
	int chip_1_total, chip_2_total, chip_3_total, chip_4_total;
	int bettingPlayerNumber;
	double chip_1_value, chip_2_value, chip_3_value, chip_4_value;
	double bigBlind, smallBlind, buyIn, potTotal, tempPot, minToCall,potLastBet;
	int bettingRound;
	Player* bettingPlayer;
	int callCount;
	bool bettingBypass;

	Game(int numPlayers, double sB, double bB, double buy);
	~Game(void);

	void getHands();

	void start();
	bool allPlayersConnected();
	int numActive();
	void updatePlayer(int playerNum);
	double readWeightSensor();
	bool areBlindsSatisfied();
	double calculatePot();
	void initializeCameras();
	void initializeCamerasToZero();
	int numberStillInRound();
	int numberAbleToBet();
	int numberAllIn();
	void resetNewHand();
	int rRank();
	int rSuit();
	void calcActiveHands();
	int calcHighestRank();
	int numberWithHighestRank(int highestRank);
};

