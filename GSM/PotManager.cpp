#include "PotManager.h"


PotManager::PotManager(vector<Player>* allPlayersIn, double* minToCallIn)
{
	allPlayers = allPlayersIn;
	minCall = minToCallIn;
}


PotManager::~PotManager(void)
{
}

void PotManager::makeMainPot()
{
	if(pot.size()>0) cout << "Error: Main pot already exists!!!\n";
	else
	{
		pot.push_back(Pot("Main"));
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
		string temp = "Sidepot " + to_string(pot.size());
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

void PotManager::rmSidePot(int sidePotNumber)
{
	pot[sidePotNumber-1].ammount += pot[sidePotNumber].ammount;
	pot.erase(pot.begin() + sidePotNumber);
}

void PotManager::fold(int playerNum)
{
	for(int i=0;i<pot.size();i++)
	{
		if(pot[i].rmEligiblePlayer(playerNum)) rmSidePot(i);
	}
}

void PotManager::call(double ammount)
{
	if(pot.size()==1) pot[0].addToPot(ammount);
	else if(pot.size()>1&&pot[pot.size()-1].ammount==0)
	{
		pot[pot.size()-2].addToPot(ammount);
	}
	else if(pot.size()>1&&pot[pot.size()-1].ammount>0)
	{
		pot[pot.size()-1].addToPot(ammount);
	}
}

void PotManager::raise(double ammount, int bettingRound, int playerNum)
{
	if(pot.size()==1) pot[0].addToPot(ammount);
	else if(*minCall==0) pot[pot.size()-1].addToPot(ammount);
	else if(pot.size()>1&&pot[pot.size()-1].ammount==0)
	{
		pot[pot.size()-1].addToPot(ammount/2);
		pot[pot.size()-2].addToPot(ammount/2);
	}
	else if(pot.size()>1&&pot[pot.size()-1].ammount>0)
	{
		pot[pot.size()-1].addToPot(ammount);
	}
}

void PotManager::allIn(int playerNum, double ammountIn, int numCalled)
{
	//Make new sidepot
	makeSidePot();

	//Add All players to the new sidepot that aren't the one that just went all in
	for(int i= 0;i<allPlayers->size();i++)
	{
		if(allPlayers->at(i).isActive()&&
		   allPlayers->at(i).isStillInRound&&
		   !allPlayers->at(i).isAllIn)
		{
			pot[pot.size()-1].addEligiblePlayer(&allPlayers->at(i));
		}
	}

	//Determine if anything needs to be transfered to the new sidepot
	double rmFromPrevPot = 0;
	if(*minCall>ammountIn)
	{
		rmFromPrevPot = *minCall - ammountIn;
		cout << "mincall: " << *minCall << "   amountIn: " << ammountIn << endl;
		cout << "removed from pprev pot : " << rmFromPrevPot << endl;
		cout << "numcalled: " << numCalled << endl;
		pot[pot.size()-2].rmFromPot(rmFromPrevPot*numCalled);
		pot[pot.size()-1].addToPot(rmFromPrevPot*numCalled);
		pot[pot.size()-2].addToPot(ammountIn);
	}
	else if(*minCall<ammountIn)
	{
		pot[pot.size()-2].addToPot(ammountIn);
	}
	else if(*minCall==ammountIn)
	{
		pot[pot.size()-2].addToPot(ammountIn);
	}

}

void PotManager::reset()
{
	for(int i=0;i<pot.size();i++) pot[i].reset();
	pot.clear();
}

void PotManager::determineWinners()
{
	//For each pot...
	for(int i=0;i<pot.size();i++)
	{
		pot[i].sortPokerHands();
	}
}

void PotManager::distributePots()
{
	for(int i =0;i<pot.size();i++)
	{
		if(pot[i].ableToWin.size()==0)
		{
			pot.pop_back();
		}
	}
	for(int i=0;i<pot.size();i++)
	{
		pot[i].distributePot();
	}
}