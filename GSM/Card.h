#pragma once
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

class Card
{
public:

	int suit, rank;

	Card(void)
	{
		suit = -1;
		rank = -1;
	}

	Card(int rankIn, int suitIn)
	{
		rank = rankIn;
		suit = suitIn;
	}

	~Card(void)
	{
	}

	void set(int rankIn, int suitIn)
	{
		rank = rankIn;
		suit = suitIn;
	}

	void printCard()
	{
		if(rank!=-1&&suit!=-1)
		{
			if(rank==14) cout << "Ace of ";
			if(rank==13) cout << "Kind of ";
			if(rank==12) cout << "Queen of ";
			if(rank==11) cout << "Jack of ";
			if(rank==10) cout << "10 of ";
			if(rank==9) cout << "9 of ";
			if(rank==8) cout << "8 of ";
			if(rank==7) cout << "7 of ";
			if(rank==6) cout << "6 of ";
			if(rank==5) cout << "5 of ";
			if(rank==4) cout << "4 of ";
			if(rank==3) cout << "3 of ";
			if(rank==2) cout << "2 of ";

			if(suit==0) cout << "Hearts\n";
			if(suit==1) cout << "Spades\n";
			if(suit==2) cout << "Clubs\n";
			if(suit==3) cout << "Diamonds\n";
		}
	}

	bool operator==(const Card &other)
	{
		if(suit==other.suit&&rank==other.rank) return true;
		else return false;
	}

	bool operator!=(const Card &other)
	{
		if(suit!=other.suit||rank!=other.rank) return true;
		else return false;
	}

	void operator=(const Card &other)
	{
		rank = other.rank;
		suit = other.suit;
	}

	bool operator<(const Card &other)
	{
		if(rank<other.rank) return true;
		else return false;
	}


};

