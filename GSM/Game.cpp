#include "Game.h"
#include "TCPserver.h"
//Branch Test Comment 

Game::Game(int numPlayers,double sB, double bB, double buy)
{
	numberOfPlayers = numPlayers;
	smallBlind = sB;
	bigBlind = bB;
	buyIn = buy;
	chip_1_value = sB;
	chip_2_value = bB;
	chip_3_value = 2*bB;
	chip_4_value = 10*bB;
	potTotal = 0;
	chip_1_total = 0;
	chip_2_total = 0;
	chip_3_total = 0;
	chip_4_total = 0;
	bettingRound = 1;
	bettingPlayer = NULL;
	callCount = 0;
	minToCall= bB;
	bettingBypass = false;
	cout << "Number of Players: " << numPlayers << endl;
	cout << "Buy in: " << buyIn << "   Small Blind: " << smallBlind << "   Big Blind: " << bigBlind << endl;
	cout << "Denominations: " << chip_1_value << "   " << chip_2_value << "   " << chip_3_value << "   " << chip_4_value << endl;


	for(int i=0;i<numPlayers;i++) player.push_back(Player());
	initSDL();
	createServer(numPlayers);

	//Waits for all players to be connected and puts them in order
	while(!allPlayersConnected())
	{
		int num = acceptConnections();
		if(num!= -1&&num<numPlayers)
		{
			player[num].setPlayerNumber(num);
			cout << "Player " << num << " connected.\n";
		}
	}
	cout << "All Players Connected!!!\n";

	//Initialize commCards
	commCard[0] = Card(0,0);
	commCard[1] = Card(0,0);
	commCard[2] = Card(0,0);
	commCard[3] = Card(0,0);
	commCard[4] = Card(0,0);

	//Initialize Cameras
	//initializeCameras();
	initializeCamerasToZero();



	cout << "All Cameras Initialized.\n";

	state = START;
	cout << "Current State: " << state << endl;
}

Game::~Game(void)
{
}

void Game::initializeCameras()
{
	cout << "Initializing Player Cameras...\n";
	for(int i=0;i<numberOfPlayers;i++)
	{
		player[i].initCamera(player[i].getPlayerNumber());
	}
	cout << "All Cameras Initialized.\n";
}

void Game::initializeCamerasToZero()
{
	cout << "Initializing all cameras to ZERO...\n";
	for(int i=0;i<numberOfPlayers;i++)
	{
		player[i].initCamera(0);
	}
	cout << "All Cameras Initialized to ZERO.\n";

}

void Game::getHands()
{
	for(int i=0;i<player.size();i++)
	{
		
	}
}

bool Game::allPlayersConnected()
{
	bool allConnected = true;
	for(int i=0;i<player.size();i++) 
	{
		if(player[i].getPlayerNumber() == -1) allConnected = false;
	}
	return allConnected;
}

int Game::numActive()
{
	int activeCount = 0;
	for(int i=0;i<numberOfPlayers;i++)
	{
		if(player[i].isActive()) activeCount++;
	}
	return activeCount;
}

void Game::updatePlayer(int playerNum)
{
	string buildMsg;
	//Hand 1
	buildMsg += to_string(player[playerNum].hand[0].suit) + " " + to_string(player[playerNum].hand[0].rank) + " ";

	//Hand 2
	buildMsg += to_string(player[playerNum].hand[1].suit) + " " + to_string(player[playerNum].hand[1].rank) + " ";

	//Comm Card 1
	buildMsg += to_string(commCard[0].suit) + " " + to_string(commCard[0].rank) + " ";

	//Comm Card 2 
	buildMsg += to_string(commCard[1].suit) + " " + to_string(commCard[1].rank) + " ";

	//Comm Card 3 
	buildMsg += to_string(commCard[2].suit) + " " + to_string(commCard[2].rank) + " ";

	//Comm Card 4 
	buildMsg += to_string(commCard[3].suit) + " " + to_string(commCard[3].rank) + " ";

	//Comm Card 5 
	buildMsg += to_string(commCard[4].suit) + " " + to_string(commCard[4].rank) + " ";

	//State
	buildMsg += to_string(state) + " ";

	//Dealer
	buildMsg += to_string(player[playerNum].isDealer()) + " ";

	//Big Blind
	buildMsg += to_string(player[playerNum].isBigBlind()) + " ";

	//Small Blind
	buildMsg += to_string(player[playerNum].isSmallBlind()) + " ";

	//isBetting
	buildMsg += to_string(player[playerNum].isBetting()) + " ";

	//Pot Total
	buildMsg += to_string(potTotal) + " ";

	//Player Ammount
	buildMsg += to_string(player[playerNum].chipTotal) + " ";

	//MinToCall
	buildMsg += to_string(minToCall-player[playerNum].lastCurrentBet) + " ";

	//Current Bet
	buildMsg += to_string(player[playerNum].currentBet - player[playerNum].lastCurrentBet) + " ";

	//MinToRaise
	buildMsg += to_string((minToCall*2)-player[playerNum].lastCurrentBet) + " ";

	//isActive
	buildMsg += to_string(player[playerNum].isActive()) + " \n";

	
	char* outMsg = &buildMsg[0u];
	//cout << outMsg << endl;

	if(sendClient(playerNum,outMsg)) cout << "Updated Player " << playerNum << endl;
}

double Game::calculatePot()
{
	return (chip_1_total*chip_1_value + chip_2_total*chip_2_value + chip_3_total*chip_3_value + chip_4_total*chip_4_value); 
}

double Game::readWeightSensor()
{
	//Read playerNumber from file
	ifstream data;
	data.open("pot.txt");
	while(!data.eof())
	{
		data >> chip_1_total;
		data >> chip_2_total;
		data >> chip_3_total;
		data >> chip_4_total;
	}
	data.close();
	//cout << chip_1_total << "   " << chip_2_total << "   " << chip_3_total << "   " << chip_4_total << endl;
	potTotal =  calculatePot();
	//cout << "POT TOTAL: " << potTotal << endl;
	return potTotal;
}

bool Game::areBlindsSatisfied()
{
	if(readWeightSensor()==(bigBlind+smallBlind)) return true;
	else return false;
}

int Game::numberStillInRound()
{
	int c = 0;
	for(int i =0; i<numberOfPlayers;i++)
	{
		if(player[i].isActive()&&player[i].isStillInRound) c++;
	}
	return c;
}

int Game::numberAbleToBet()
{
	int c = 0;
	for(int i=0;i<numberOfPlayers;i++)
	{
		if(player[i].isActive()&&player[i].isStillInRound&&!player[i].isAllIn) c++;
	}
	return c;
}

void Game::resetNewHand()
{
	for(int i=0; i<numberOfPlayers;i++)
	{
		if(player[i].buyInNextRound==true) player[i].buyIn(20);
	}

	for(int i=0;i<numberOfPlayers;i++)
	{
		if(player[i].isDealer())
		{
			player[i].setDealer(false);
			player[i].findNextActive()->setDealer(true);
			break;
		}
	}
	for(int i=0;i<numberOfPlayers;i++)
	{
		if(player[i].isSmallBlind())
		{
			player[i].setSmallBlind(false);
		}
		if(player[i].isDealer())
		{
			player[i].findNextActive()->setSmallBlind(true);
			break;
		}
	}
	for(int i=0;i<numberOfPlayers;i++)
	{
		if(player[i].isBigBlind())
		{
			player[i].setBigBlind(false);
		}
		if(player[i].isDealer())
		{
			player[i].findNextActive()->findNextActive()->setBigBlind(true);
			break;
		}
	}

	for(int i=0;i<numberOfPlayers;i++)
	{
		player[i].hand[0].set(0,0);
		player[i].hand[1].set(0,0);
		player[i].currentBet = 0;
		player[i].lastCurrentBet = 0;
		player[i].totalPutIntoPot = 0;
		player[i].isBet = false;
		player[i].isAllIn = false;
		player[i].isStillInRound = false;
		player[i].fullHand.resetPokerHand();
		player[i].possibleWinner = false;
		player[i].buyInNextRound =false;
	}
	for(int i=0;i<5;i++) commCard[i].set(0,0);
	bettingPlayer = NULL;
	bettingRound = 1;
	bettingBypass = false;
	callCount = 0;
	deck.resetDeck();
}

void Game::start()
{
	int ident = 0;
	char* command = "asdf";

	while(true) 
	{
		switch(state)
		{
			//*********************************************START*****************************************************
			case START:
			if(getCommand(command,ident)>=0)
			{
				//Buy-in
				if(strcmp(command,"buyIn")==0)
				{
					cout << "Player " << ident << " bought in!!!\n";
					player[ident].buyIn(20);
				}
				//Start
				if(strcmp(command,"start")==0&&numActive()>1)
				{
					state = BLINDS;
					cout << "Current State: " << state << endl;

					//Set Player Pointers
					for(int i=0;i<numberOfPlayers;i++)
					{
						if(i<numberOfPlayers-1) player[i].nextPlayer = &player[i+1];
						else player[i].nextPlayer = &player[0];
					}

					//Set Initial Dealer
					for(int i=0;i<numberOfPlayers;i++)
					{
						if(player[i].isActive())
						{
							player[i].setDealer(true);
							break;
						}
					}

					//set Small Blind
					for(int i=0;i<numberOfPlayers;i++)
					{
						if(player[i].isDealer())
						{
							player[i].findNextActive()->setSmallBlind(true);
						}
					}

					//set Big Blind
					for(int i=0;i<numberOfPlayers;i++)
					{
						if(player[i].isSmallBlind())
						{
							player[i].findNextActive()->setBigBlind(true);
						}
					}
					readWeightSensor();
					//for(int i=0;i<numberOfPlayers;i++) if(player[i].isActive()) updatePlayer(i);
					for(int i=0;i<numberOfPlayers;i++) updatePlayer(i);
					SDL_Delay(250);
				}
			}
			
			break;

			//*********************************************BLINDS*****************************************************
			case BLINDS:

			if(getCommand(command,ident)>=0)
			{
				//Buy-in
				if(strcmp(command,"buyIn")==0)
				{
					cout << "Player " << ident << " bought in!!!\n";
					player[ident].buyInNextRound = true;
				}
			}

			if(tempPot!=readWeightSensor())
			{
				for(int i=0;i<numberOfPlayers;i++) if(player[i].isActive()) updatePlayer(i);
				SDL_Delay(250);
			}
			tempPot = potTotal;

			if(areBlindsSatisfied())
			{
				state = DEALING;
				cout << "Current State: " << state << endl;
				//Find small blind and subtract from total
				for(int i=0;i<numberOfPlayers;i++)
				{
					if(player[i].isSmallBlind())
					{
						player[i].chipTotal -= smallBlind;
						player[i].currentBet = smallBlind;
						player[i].lastCurrentBet = 0;
						player[i].totalPutIntoPot += smallBlind;
					}
					if(player[i].isBigBlind())
					{
						player[i].chipTotal -= bigBlind;
						player[i].currentBet = bigBlind;
						player[i].lastCurrentBet = 0;
						player[i].totalPutIntoPot += bigBlind;
					}
				}
				minToCall = bigBlind;
				for(int i=0;i<numberOfPlayers;i++) if(player[i].isActive()) player[i].isStillInRound = true;
				for(int i=0;i<numberOfPlayers;i++) if(player[i].isActive()) updatePlayer(i);
				SDL_Delay(50);
			}

			break;

			//*********************************************DEALING*****************************************************
			case DEALING:
			
			cout <<  "Blinds Satisfied. Now in Dealing state\n";
			player[0].hand[0].set(12,1);
			player[0].hand[1].set(12,3);
			player[0].fullHand.addCard(player[0].hand[0]);
			player[0].fullHand.addCard(player[0].hand[1]);

			player[1].hand[0].set(10,3);
			player[1].hand[1].set(11,3);
			player[1].fullHand.addCard(player[1].hand[0]);
			player[1].fullHand.addCard(player[1].hand[1]);

			player[2].hand[0].set(4,3);
			player[2].hand[1].set(4,2);
			player[2].fullHand.addCard(player[2].hand[0]);
			player[2].fullHand.addCard(player[2].hand[1]);

			state = BETTING;
			for(int i=0;i<numberOfPlayers;i++) if(player[i].isActive()) updatePlayer(i);
			SDL_Delay(250);

			//*********************************************BETTING*****************************************************
			case BETTING:
			cout << "Now in Betting State\n";

			//If bettingRound==1, set bettingPlayer to FIRST AFTER BB
			if(bettingPlayer==NULL)
			{
				if(bettingRound==1)
				{
					for(int i=0;i<numberOfPlayers;i++)
					{
						if(player[i].isBigBlind())
						{
							bettingPlayer = player[i].findNextActive();
							bettingPlayer->setBetting(true);
							updatePlayer(bettingPlayer->getPlayerNumber());
							bettingPlayerNumber = bettingPlayer->getPlayerNumber();
							potLastBet = potTotal;
							SDL_Delay(50);
						}
					}
				}
				//If bettingRound>1, set betting player TO SB or next after SB
				else
				{
					callCount = 0;
					for(int i=0;i<numberOfPlayers;i++)
					{
						player[i].currentBet = 0;
						player[i].lastCurrentBet = 0;
						if(player[i].isSmallBlind())
						{
							if(player[i].isStillInRound)
							{
								bettingPlayer = &player[i];
								bettingPlayer->setBetting(true);
								updatePlayer(bettingPlayer->getPlayerNumber());
								bettingPlayerNumber = bettingPlayer->getPlayerNumber();
								potLastBet = potTotal;
								SDL_Delay(50);
							}
							else
							{
								bettingPlayer = player[i].findNextActiveAndInRound();
								bettingPlayer->setBetting(true);
								updatePlayer(bettingPlayer->getPlayerNumber());
								bettingPlayerNumber = bettingPlayer->getPlayerNumber();
								potLastBet = potTotal;
								SDL_Delay(50);
							}
						}
						
					}
				}
			}

			//While Player is betting look for commands
			while(player[bettingPlayerNumber].isBetting())
			{
				//Update if chips change
				if(tempPot!=readWeightSensor())
				{

					if(potTotal>tempPot)
					{
					if(bettingRound==1) player[bettingPlayerNumber].currentBet = potTotal - potLastBet + player[bettingPlayerNumber].lastCurrentBet;
					else player[bettingPlayerNumber].currentBet = potTotal - potLastBet + player[bettingPlayerNumber].lastCurrentBet;
					//updatePlayer(bettingPlayerNumber);
					//SDL_Delay(100);
					for(int i=0;i<numberOfPlayers;i++) if(player[i].isActive()) updatePlayer(i);
					SDL_Delay(50);
					}
					else
					{
						if(bettingRound==1) player[bettingPlayerNumber].currentBet = potTotal - potLastBet + player[bettingPlayerNumber].lastCurrentBet;
						else player[bettingPlayerNumber].currentBet = potTotal - potLastBet + player[bettingPlayerNumber].lastCurrentBet;
						//updatePlayer(bettingPlayerNumber);
						//SDL_Delay(100);
						for(int i=0;i<numberOfPlayers;i++) if(player[i].isActive()) updatePlayer(i);
						SDL_Delay(50);
					}
				}
				tempPot = potTotal;

				//Get commands from UI
				if(getCommand(command,ident)>=0)
				{
					if(strcmp(command,"bet")==0)
					{
						//if(bettingRound==1) player[bettingPlayerNumber].currentBet += potTotal - potLastBet;
						//else player[bettingPlayerNumber].currentBet += potTotal - potLastBet;
						//updatePlayer(bettingPlayerNumber);
						//SDL_Delay(100);

						///////////////////////////////////////COULD HAVE ALL IN BUTTON///////////////////////////
						//Player went all in
						if((player[bettingPlayerNumber].currentBet-player[bettingPlayerNumber].lastCurrentBet)==player[bettingPlayerNumber].chipTotal)
						{
							player[bettingPlayerNumber].totalPutIntoPot += player[bettingPlayerNumber].currentBet - player[bettingPlayerNumber].lastCurrentBet;
							callCount++;
							cout << "Player " << bettingPlayerNumber << " went all in for $" << player[bettingPlayerNumber].currentBet << endl;
							player[bettingPlayerNumber].chipTotal -= player[bettingPlayerNumber].currentBet - player[bettingPlayerNumber].lastCurrentBet;
							if(player[bettingPlayerNumber].chipTotal == 0) player[bettingPlayerNumber].isAllIn = true;
							//minToCall = player[bettingPlayerNumber].currentBet;
							player[bettingPlayerNumber].setBetting(false);
							updatePlayer(bettingPlayerNumber);
							bettingPlayerNumber = player[bettingPlayerNumber].findNextActiveAndInRound()->getPlayerNumber();
							if(callCount != numberStillInRound()) player[bettingPlayerNumber].setBetting(true);
							potLastBet = potTotal;
							player[bettingPlayerNumber].lastCurrentBet = player[bettingPlayerNumber].currentBet;
							updatePlayer(bettingPlayerNumber);
							SDL_Delay(50);
						}
						//Player Called
						else if(player[bettingPlayerNumber].currentBet == minToCall&&player[bettingPlayerNumber].currentBet<=player[bettingPlayerNumber].chipTotal)
						{
							player[bettingPlayerNumber].totalPutIntoPot += player[bettingPlayerNumber].currentBet - player[bettingPlayerNumber].lastCurrentBet;
							callCount++;
							cout << "Player " << bettingPlayerNumber << " called for $" << player[bettingPlayerNumber].currentBet << endl;
							player[bettingPlayerNumber].chipTotal -= player[bettingPlayerNumber].currentBet - player[bettingPlayerNumber].lastCurrentBet;
							if(player[bettingPlayerNumber].chipTotal == 0) player[bettingPlayerNumber].isAllIn = true;
							player[bettingPlayerNumber].setBetting(false);
							updatePlayer(bettingPlayerNumber);
							SDL_Delay(50);
							bettingPlayerNumber = player[bettingPlayerNumber].findNextActiveAndInRound()->getPlayerNumber();
							if(callCount != numberStillInRound()) player[bettingPlayerNumber].setBetting(true);
							potLastBet = potTotal;
							player[bettingPlayerNumber].lastCurrentBet = player[bettingPlayerNumber].currentBet;
							updatePlayer(bettingPlayerNumber);
							SDL_Delay(50);
						}

						//Player Raised
						else if(player[bettingPlayerNumber].currentBet >= 2*minToCall&&player[bettingPlayerNumber].currentBet<=player[bettingPlayerNumber].chipTotal)
						{
							player[bettingPlayerNumber].totalPutIntoPot += player[bettingPlayerNumber].currentBet - player[bettingPlayerNumber].lastCurrentBet;
							callCount = 1;
							cout << "Player " << bettingPlayerNumber << " raised $" << player[bettingPlayerNumber].currentBet-minToCall << endl;
							player[bettingPlayerNumber].chipTotal -= player[bettingPlayerNumber].currentBet - player[bettingPlayerNumber].lastCurrentBet;
							if(player[bettingPlayerNumber].chipTotal == 0) player[bettingPlayerNumber].isAllIn = true;
							minToCall = player[bettingPlayerNumber].currentBet;
							player[bettingPlayerNumber].setBetting(false);
							updatePlayer(bettingPlayerNumber);
							SDL_Delay(50);
							bettingPlayerNumber = player[bettingPlayerNumber].findNextActiveAndInRound()->getPlayerNumber();
							if(callCount != numberStillInRound()) player[bettingPlayerNumber].setBetting(true);
							potLastBet = potTotal;
							player[bettingPlayerNumber].lastCurrentBet = player[bettingPlayerNumber].currentBet;
							updatePlayer(bettingPlayerNumber);
							SDL_Delay(50);
						}
					}

					if(strcmp(command,"fold")==0)
					{
						cout << "Player " << bettingPlayerNumber << " folded!\n";
						player[bettingPlayerNumber].isStillInRound = false;
						player[bettingPlayerNumber].setBetting(false);
						updatePlayer(bettingPlayerNumber);
						SDL_Delay(50);
						bettingPlayerNumber = player[bettingPlayerNumber].findNextActiveAndInRound()->getPlayerNumber();
						if(callCount != numberStillInRound()&&numberAbleToBet()>1) player[bettingPlayerNumber].setBetting(true);
						potLastBet = potTotal;
						player[bettingPlayerNumber].lastCurrentBet = player[bettingPlayerNumber].currentBet;
						updatePlayer(bettingPlayerNumber);
						SDL_Delay(50);
					}

					//Buy-in
					if(strcmp(command,"buyIn")==0)
					{
						cout << "Player " << ident << " bought in!!!\n";
						player[ident].buyInNextRound = true;
					}

				}

			//If all called, end the round of betting
			if(callCount == numberStillInRound())
			{
				bettingRound++;
				cout << "BETTING ROUND: " << bettingRound << endl;
				minToCall = 0;
				player[bettingPlayerNumber].setBetting(false);
				for(int i=0;i<numberOfPlayers; i++) player[i].setBetting(false);
				for(int i=0;i<numberOfPlayers;i++) player[i].currentBet = 0;
				for(int i=0;i<numberOfPlayers; i++)  player[i].lastCurrentBet = 0;
				//for(int i=0;i<numberOfPlayers;i++) if(player[i].isActive()) updatePlayer(i);
				//bettingPlayerNumber=0;
				if(bettingRound<5)state = COMMCARD;
				else state = HANDRES;
				SDL_Delay(100);
				//break;
			}

			//If only 1 still in round
			if(numberStillInRound()==1)
			{
				for(int i=0;i<numberOfPlayers; i++) player[i].setBetting(false);
				updatePlayer(bettingPlayerNumber);
				state = HANDRES;
				SDL_Delay(50);
			}

			//Skip Future betting
			if(numberStillInRound()>1&&numberAbleToBet()==1)
			{
				//bettingRound++;
				for(int i=0;i<numberOfPlayers; i++) player[i].setBetting(false);
				cout << "BYPASS!!!\n"  << endl;
				bettingBypass = true;
				state = COMMCARD;
				SDL_Delay(50);
			}

			}

			//state = COMMCARD;
			SDL_Delay(50);
			break;
			//*********************************************COMMUNITY CARDS*****************************************************
			case COMMCARD:

			if(getCommand(command,ident)>=0)
			{
				//Buy-in
				if(strcmp(command,"buyIn")==0)
				{
					cout << "Player " << ident << " bought in!!!\n";
					player[ident].buyIn(20);
				}
			}

			cout << "Now in Community Card State\n";
			if(bettingRound==2)
			{
				commCard[0].set(5,1);
				commCard[1].set(5,3);
				commCard[2].set(9,1);
				player[0].fullHand.addCard(commCard[0]);
				player[0].fullHand.addCard(commCard[1]);
				player[0].fullHand.addCard(commCard[2]);
				player[1].fullHand.addCard(commCard[0]);
				player[1].fullHand.addCard(commCard[1]);
				player[1].fullHand.addCard(commCard[2]);
				player[2].fullHand.addCard(commCard[0]);
				player[2].fullHand.addCard(commCard[1]);
				player[2].fullHand.addCard(commCard[2]);
				if(bettingBypass) bettingRound++;
			}

			if(bettingRound==3)
			{
				commCard[3].set(10,2);
				player[0].fullHand.addCard(commCard[3]);
				player[1].fullHand.addCard(commCard[3]);
				player[2].fullHand.addCard(commCard[3]);
				if(bettingBypass) bettingRound++;
			}

			if(bettingRound==4)
			{
				commCard[4].set(11,0);
				player[0].fullHand.addCard(commCard[4]);
				player[1].fullHand.addCard(commCard[4]);
				player[2].fullHand.addCard(commCard[4]);
				if(bettingBypass) state = HANDRES;
			}

			if(!bettingBypass)
			{
				state = BETTING;
				for(int i=0;i<numberOfPlayers;i++) if(player[i].isActive()) updatePlayer(i);
				bettingPlayer = NULL;
				SDL_Delay(50);
			}
			if(bettingBypass) for(int i=0;i<numberOfPlayers;i++) updatePlayer(i);
			break;

			//*********************************************HAND RES*****************************************************
			case HANDRES:
			{
			
			if(getCommand(command,ident)>=0)
			{
				//Buy-in
				if(strcmp(command,"buyIn")==0)
				{
					cout << "Player " << ident << " bought in!!!\n";
					player[ident].buyIn(20);
				}
			}

			cout << "Now in Hand Resolution State\n";

			int highestRank = 0;
			int numWithHighestRank = 0;

			//Calculate Hands for each player still in game
			// and determine the highest rank among players and how many have it
			for(int i=0;i<numberOfPlayers;i++)
			{
				if(player[i].isActive()&&player[i].isStillInRound)
				{
					player[i].fullHand.calcHandRank();
					//player[i].fullHand.printBestHand();
					if(player[i].fullHand.handRank>highestRank&&player[i].isStillInRound)
					{
						highestRank = player[i].fullHand.handRank;
						numWithHighestRank = 1;
					}
					else if(player[i].fullHand.handRank==highestRank&&player[i].isStillInRound)
					{
						numWithHighestRank++;
					}
				}
			}

			//Flag Possible Winning Players
			for(int i=0;i<numberOfPlayers;i++)
			{
				if(player[i].fullHand.handRank == highestRank&&player[i].isStillInRound)
				{
					player[i].possibleWinner=true;
					cout << "Player " << i << " possible winner\n";
				}
				
			}

			vector<Player*> winner;
			//If only 1 possible winner
			if(numWithHighestRank==1)
			{
				int winningPlayerNumber;
				for(int i=0;i<numberOfPlayers;i++) if(player[i].possibleWinner) winningPlayerNumber = i;
				cout << "Player " << winningPlayerNumber << " wins with\n";
				player[winningPlayerNumber].fullHand.printBestHand();
				winner.push_back(&player[winningPlayerNumber]);
			}
			//2 or more possible winners
			else
			{
				vector<Player*> unsortedPossibleWinner;
				for(int i=0;i<numberOfPlayers;i++)
				{
					if(player[i].possibleWinner) unsortedPossibleWinner.push_back(&player[i]);
				}

				for(int i=0;i<unsortedPossibleWinner.size();i++) unsortedPossibleWinner[i]->fullHand.printBestHand();
				if(player[1].fullHand<player[0].fullHand) cout <<  "1<0";
				if(player[0].fullHand<player[1].fullHand) cout <<  "1<2";

				Player* highest = unsortedPossibleWinner[0];
				winner.push_back(unsortedPossibleWinner[0]);
				for(int i=1;i<unsortedPossibleWinner.size();i++)
				{
					if(highest->fullHand < unsortedPossibleWinner[i]->fullHand&&!(highest->fullHand==unsortedPossibleWinner[i]->fullHand))
					{
						winner.clear();
						highest = unsortedPossibleWinner[i];
						winner.push_back(unsortedPossibleWinner[i]);
					}
					else if(highest->fullHand==unsortedPossibleWinner[i]->fullHand)
					{
						winner.push_back(unsortedPossibleWinner[i]);
					}
				}

				cout << "Number of Winners: " << winner.size() << endl;
				for(int i=0;i<winner.size();i++)
				{
					cout << "Player " << winner[i]->getPlayerNumber() << " won\n";
					winner[i]->fullHand.printBestHand();
				}
			}

			//1 Winner
			if(winner.size()==1||numWithHighestRank==1)
			{
				winner[0]->chipTotal+=potTotal;
				updatePlayer(winner[0]->getPlayerNumber());
				SDL_Delay(50);
			}
			//More than 1 Winner
			else
			{
				for(int i=0;i<winner.size();i++)
				{
					winner[i]->chipTotal += potTotal/(winner.size());
					updatePlayer(i);
					SDL_Delay(50);
				}
			}

			state = CLEANUP;
			break;
			}
			//*********************************************CLEAN UP*****************************************************
			case CLEANUP:

			if(getCommand(command,ident)>=0)
			{
				//Buy-in
				if(strcmp(command,"buyIn")==0)
				{
					cout << "Player " << ident << " bought in!!!\n";
					player[ident].buyIn(20);
				}
			}

			if(readWeightSensor()==0)
			{
				resetNewHand();
				state=BLINDS;
			}

			SDL_Delay(50);
		}
	}
}

