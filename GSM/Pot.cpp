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

void Pot::rmFromPot(double ammountIn)
{
	ammount -= ammountIn;
}

void Pot::printPot()
{
	cout << "     " << name << ": " << ammount << " - ";
	for(int i=0;i<ableToWin.size();i++)
	{
		cout << ableToWin[i]->getPlayerNumber() << " ";
	}
	cout << endl;
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

void Pot::sortPokerHands()
{
	cout << "////////// Sorting Pot " << name << " //////////" << endl;

	//Put players into sorted vector
	for(int i=0;i<ableToWin.size();i++)
	{
		sorted.push_back(*ableToWin.at(i));
	}

	sort(sorted.begin(),sorted.end());
	cout << "Sorted PokerHands (low to high):  ";
	for(int i=0;i<sorted.size();i++) cout << "Player " << sorted[i].getPlayerNumber() << "  ";
	cout << endl;

	//cout << "////////// Sorting Pot " << name << " End //////////" << endl;
}

void Pot::distributePot()
{
	cout << "////////// Distributing Pot " << name << " //////////////" << endl;

	int winnerCount = 1;
	for(int i=sorted.size()-1;i>0;i--)
	{
		if(sorted[i].fullHand==sorted[i-1].fullHand) winnerCount++;
	}

	vector<int> winningPlayerNumbers;
	for(int i=0;i<winnerCount;i++)
	{
		winningPlayerNumbers.push_back(sorted[sorted.size()-1+i].getPlayerNumber());
	}
	for(int i=0;i<winningPlayerNumbers.size();i++) cout << winningPlayerNumbers[i] << "  ";
	cout << endl;

	for(int i=0;i<winningPlayerNumbers.size();i++)
	{
		for(int j=0;j<ableToWin.size();j++)
		{
			if(ableToWin.at(j)->getPlayerNumber()==winningPlayerNumbers[i]) winners.push_back(ableToWin.at(j));
		}
	}

	double ammountAwarded = ammount/winnerCount;
	for(int i=0;i<winners.size();i++)
	{
		winners.at(i)->chipTotal += ammountAwarded;
		cout << "Player " << winners.at(i)->getPlayerNumber() << " awarded $" << ammountAwarded << endl;
	}

}
