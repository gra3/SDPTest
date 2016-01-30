#include "Pot.h"


Pot::Pot(string nameIn)
{
	ammount =0;
	name = nameIn;
}

Pot::~Pot(void)
{
}

void Pot::addToPot(double ammountIn)
{
	ammount += ammountIn;
}

void Pot::printPot()
{
	cout << "     " << name << ": " << ammount << " - ";
	for(int i=0;i<ableToWin.size();i++)
	{
		cout << ableToWin[i]->getPlayerNumber() << " ";
	}
}

void Pot::addEligiblePlayer(Player* playerIn)
{
	ableToWin.push_back(playerIn);
}

void Pot::rmEligiblePlayer(int playerNum)
{
	int toBeRemoved = -1;
	for(int i=0;i<ableToWin.size();i++)
	{
		if(ableToWin[i]->getPlayerNumber()==playerNum) toBeRemoved = i;
	}
	if(toBeRemoved==-1) cout << "Error: Player: " << playerNum << " DNE in pot " << name << "!!!\n";
	else
	{
		ableToWin.erase(ableToWin.begin()+toBeRemoved);
	}
}
