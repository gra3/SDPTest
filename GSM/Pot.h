#pragma once
#include "Player.h"

class Pot
{
public:
	Pot(string nameIn);
	~Pot(void);

	int ammount;
	string name;
	vector<Player*> ableToWin;
	vector<Player*> winners;
	void addToPot(int ammountIn);
	void printPot();
};

