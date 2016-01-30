#include "PotManager.h"


PotManager::PotManager(vector<Player>* allPlayersIn)
{
	allPlayers = allPlayersIn;
}


PotManager::~PotManager(void)
{
}

void PotManager::makeMainPot()
{
	if(pot.size()>0) cout << "Error: Main pot already exists!!!\n";
	else
	{
		pot.push_back(Pot("main"));
	}
	for(int i=0;i<allPlayers->size();i++)
	{
		if(allPlayers->at(i).isActive()&&allPlayers->at(i).isStillInRound) pot[0].addEligiblePlayer(&allPlayers->at(i));
	}
}

void PotManager::makeSidePot()
{
	if(pot.size()==0) cout << "Error: No main pot exists!!!\n";
	else
	{
		string temp = "sidepot " + to_string(pot.size()+1);
		pot.push_back(Pot(temp));
	}
}

void PotManager::printPots()
{
	cout << "/////////// Pot Status //////////\n";
	for(int i=0;i<pot.size();i++)
	{
		pot[i].printPot();
	}
	cout << endl;
}

void PotManager::add(double ammountIn)
{
	if(pot.size()==0) cout << "Error: No pots created!!!\n";
	else if(pot.size()==1)
	{
		pot[0].addToPot(ammountIn);
	}
}

void PotManager::fold(int playerNum)
{
	for(int i=0;i<pot.size();i++)
	{
		pot[i].rmEligiblePlayer(playerNum);
	}
}