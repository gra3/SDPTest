#pragma once
#include "Player.h"

class Pot
{
public:
	Pot(string nameIn);
	~Pot(void);

	double ammount;
	string name;
	vector<Player*> ableToWin;
	vector<Player*> winners;
	void addToPot(double ammountIn);
	void printPot();
	void addEligiblePlayer(Player* playerIn);
	void rmEligiblePlayer(int playerNum);
};

