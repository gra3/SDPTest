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

void PotManager::call(double ammount)
{
	if(pot.size()==1) pot[0].addToPot(ammount);
	else if(pot.size()>1&&pot[pot.size()-1].ammount==0)
	{
		pot[pot.size()-2].addToPot(ammount);
	}
	else if(pot.size()>1&&pot[pot.size()-1].ammount!=0)
	{
		pot[pot.size()-1].addToPot(ammount);
	}
}

void PotManager::raise(double ammount, int bettingRound, int playerNum)
{
	double blindAdjust = 0;
	if(bettingRound==1)
	{
		if(allPlayers->at(playerNum).isSmallBlind()) blindAdjust = .25;
		else if(allPlayers->at(playerNum).isBigBlind()) blindAdjust = .5;
	}
	if(pot.size()==1) pot[0].addToPot(ammount);
	else if(pot.size()>1)
	{
		pot[pot.size()-1].addToPot(ammount-*minCall);
		pot[pot.size()-2].addToPot(ammount-*minCall+blindAdjust);
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
		pot[pot.size()-2].rmFromPot(rmFromPrevPot);
		pot[pot.size()-1].addToPot(rmFromPrevPot);
	}
	else if(*minCall<ammountIn)
	{
		pot[pot.size()-2].addToPot(ammountIn);
	}
	else if(*minCall==ammountIn)
	{
		pot[pot.size()-1].addToPot(ammountIn);
	}

}

void PotManager::reset()
{
	pot.clear();
}