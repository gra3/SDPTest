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