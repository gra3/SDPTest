#include "Player.h"


Player::Player()
{
	playerNumber = -1;
	cam = new CardDetect(0);
	chipTotal = 0;
	active = false;
	hand[0] = Card(0,0);
	hand[1] = Card(0,0);
	isD = false;
	isBB = false;
	isSB = false;
	isBet = false;
	currentBet = 0;
	lastCurrentBet = 0;
	isStillInRound = false;
	isAllIn = false;
	possibleWinner = false;
	totalPutIntoPot = 0;
	buyInNextRound = false;
}


Player::~Player(void)
{
	//delete cam;
}

void Player::initCamera(int camNum)
{
	cam = new CardDetect(camNum);
}

void Player::getHand()
{

}

void Player::setPlayerNumber(int playerNum)
{
	playerNumber = playerNum;
}

int Player::getPlayerNumber()
{
	return playerNumber;
}

bool Player::isActive()
{
	return active;
}

void Player::setActive()
{
	active = true;
}

void Player::setInactive()
{
	active = false;
}

void Player::buyIn(double ammount)
{
	setActive();
	chipTotal = ammount;
}

bool Player::isDealer()
{
	return isD;
}

void Player::setDealer(bool val)
{
	isD = val;
}

bool Player::isBigBlind()
{
	return isBB;
}

void Player::setBigBlind(bool val)
{
	isBB = val;
}

bool Player::isSmallBlind()
{
	return isSB;
}

void Player::setSmallBlind(bool val)
{
	isSB = val;
}

bool Player::isBetting()
{
	return isBet;
}

void Player::setBetting(bool val)
{
	isBet = val;
}

bool Player::bothHandCardsDealt()
{
	if(hand[0].rank!=0&&hand[1].rank!=0) return true;
	else return false;
}

int Player::numberOfHandCards()
{
	int c = 0;
	if(hand[0].rank!=0) c++;
	if(hand[1].rank!=0) c++;
	return c;
}

Player* Player::findNextActive()
{
	Player* nextActive;
	nextActive = nextPlayer;
	while(!nextActive->isActive())
	{
		nextActive = nextActive->nextPlayer;
	}
	return nextActive;
}

Player* Player::findNextActiveAndInRound()
{
	Player* nextActive;
	nextActive = nextPlayer;
	while(!nextActive->isActive()||!nextActive->isStillInRound||nextActive->isAllIn)
	{
		nextActive = nextActive->nextPlayer;
	}
	return nextActive;
}

bool Player::operator<(const Player &other)
{
	if(fullHand.hand.size()==7)
	{
		if(fullHand<other.fullHand) return true;
		else return false;
	}
	else 
	{
		cout << "Error Player::operator<   sorting without 7 cards\n";
		return false;
	}
}
