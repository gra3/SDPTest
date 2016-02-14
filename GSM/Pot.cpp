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

//returns true if pot needs deleted
bool Pot::rmEligiblePlayer(int playerNum)
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
	if(ableToWin.size()==0) return true;
	else return false;
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

	if(ableToWin.size()==1) 
	{
		ableToWin.at(0)->chipTotal += ammount;
		ableToWin.at(0)->addWinnings(ammount);
		cout << "Player " << ableToWin.at(0)->getPlayerNumber() << " awarded $" << ammount << endl;
	}
	else
	{
		cout << "Winner Count\n";
		int winnerCount = 1;
		for(int i=sorted.size()-1;i>0;i--)
		{
			if(sorted[sorted.size()-1].fullHand==sorted[i-1].fullHand) winnerCount++;
		}
		cout << "Winner Count equals === " << winnerCount << endl;
		cout << "Sorted size()    " << sorted.size() << endl;
		cout << "Getting Winning Player Numbers\n";
		vector<int> winningPlayerNumbers;
		for(int i=0;i<winnerCount;i++)
		{
			cout << "i = " << i << endl;
			cout << "sorted.size()-1+i  =  " << sorted.size()-1+i << endl;
			winningPlayerNumbers.push_back(sorted[sorted.size()-1-i].getPlayerNumber());
		}

		cout << "Making vector of winners\n";
		for(int i=0;i<winningPlayerNumbers.size();i++)
		{
			for(int j=0;j<ableToWin.size();j++)
			{
				if(ableToWin.at(j)->getPlayerNumber()==winningPlayerNumbers[i]) winners.push_back(ableToWin.at(j));
			}
		}

		double ammountAwarded = 0;
		double unevenSplitBonus = 0;
		cout << "Splitting pot amongst winners\n";
		if(fmod(ammount/winnerCount,0.25)!=0)
		{
			unevenSplitBonus = fmod(ammount/winnerCount,0.25)*winnerCount;
			cout << "Uneven Split: " << unevenSplitBonus << endl;
			ammountAwarded = (ammount-unevenSplitBonus)/winnerCount;
			
		}
		else ammountAwarded = ammount/winnerCount;

		for(int i=0;i<winners.size();i++)
		{
			winners.at(i)->chipTotal += ammountAwarded;
			winners.at(i)->addWinnings(ammountAwarded);
			cout << "Player " << winners.at(i)->getPlayerNumber() << " awarded $" << ammountAwarded << endl;
		}

		if(unevenSplitBonus>0)
		{
			int randPlayer = rand()%winners.size();
			winners.at(randPlayer)->chipTotal += unevenSplitBonus;
			winners.at(randPlayer)->addWinnings(unevenSplitBonus);
			cout << "Player " << winners.at(randPlayer)->getPlayerNumber() << " gets uneven split bonus of $" << unevenSplitBonus << endl;
		}
	}
}

void Pot::reset()
{
	ableToWin.clear();
	winners.clear();
	sorted.clear();
}