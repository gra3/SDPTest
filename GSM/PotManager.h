#pragma once
#include "Pot.h"

class PotManager
{
public:
	PotManager(vector<Player>* allPlayersIn,double* minToCallIn);
	~PotManager(void);

	vector<Player>* allPlayers;
	vector<Pot> pot;
	double* minCall;

	void makeMainPot();
	void makeSidePot();
	void printPots();
	void add(double ammountIn);
	void fold(int playerNum);
	void allIn(int playerNum, double ammountIn, int numCalled);
	void reset();
	void call(double ammount);
	void raise(double ammount, int bettingRound, int playerNum);
};

