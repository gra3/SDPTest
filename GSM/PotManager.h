#pragma once
#include "Pot.h"

class PotManager
{
public:
	PotManager(vector<Player>* allPlayersIn);
	~PotManager(void);

	vector<Player>* allPlayers;
	vector<Pot> pot;
	void makeMainPot();
	void makeSidePot();
	void printPots();
	void add(double ammountIn);
	void fold(int playerNum);
};

