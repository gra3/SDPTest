#include "PokerHand.h"


PokerHand::PokerHand(void)
{
	handRank = 0;
	for(int i=0;i<15;i++) rankCountArray[i] = 0;
	for(int i=0;i<4;i++) suitCountArray[i] = 0;
	highCard = 0;
	kicker = 0;
}


PokerHand::~PokerHand(void)
{
}

void PokerHand::addCard(Card cardIn)
{
	hand.push_back(cardIn);
	rankCountArray[cardIn.rank] += 1;
	if(cardIn.rank == 14) rankCountArray[1] += 1;
	suitCountArray[cardIn.suit] += 1;
}

void PokerHand::printArrays()
{
	cout << "Rank Array\n";
	for(int i=2;i<15;i++) cout << i << ": " << rankCountArray[i] << endl;

	cout << "Suit Array\n";
	for(int i=0;i<4;i++) cout << i << ": " << suitCountArray[i] << endl;
}

bool PokerHand::flushDetected(vector<Card> &handIn)
{
	if( handIn[0].suit==handIn[1].suit&&
		handIn[0].suit==handIn[2].suit&&
		handIn[0].suit==handIn[3].suit&&
		handIn[0].suit==handIn[4].suit) return true;
	else return false;
}

bool PokerHand::straightDetected(vector<Card> &handIn)
{
	//General Case
	if( handIn[4].rank==handIn[3].rank+1&&
		handIn[4].rank==handIn[2].rank+2&&
		handIn[4].rank==handIn[1].rank+3&&
		handIn[4].rank==handIn[0].rank+4) return true;
	//Ace through 5
	else if(handIn[4].rank==14&&
			handIn[3].rank==5&&
			handIn[2].rank==4&&
			handIn[1].rank==3&&
			handIn[0].rank==2) return true;
	else return false;
}

bool PokerHand::royalFlush(vector<Card> &handIn)
{
	if( flushDetected(handIn)&&
		handIn[4].rank==14&&
		handIn[3].rank==13&&
		handIn[2].rank==12&&
		handIn[1].rank==11&&
		handIn[0].rank==10) return true;
	else return false;
}

bool PokerHand::fourOfAKind(vector<Card> &handIn)
{
	if( handIn[0].rank == handIn[1].rank&&
		handIn[0].rank == handIn[2].rank&&
		handIn[0].rank == handIn[3].rank) return true;
	else if(handIn[1].rank == handIn[2].rank&&
			handIn[1].rank == handIn[2].rank&&
			handIn[1].rank == handIn[4].rank) return true;
	else return false;
}

bool PokerHand::threeOfAKind(vector<Card> &handIn)
{
	if( handIn[0].rank == handIn[1].rank&&
		handIn[0].rank == handIn[2].rank) return true;
	else if(handIn[2].rank == handIn[3].rank&&
			handIn[2].rank == handIn[4].rank) return true;
	else return false;
}

bool PokerHand::fullHouse(vector<Card> &handIn)
{
	if( handIn[0].rank == handIn[1].rank&&
		handIn[0].rank == handIn[2].rank&&
		handIn[3].rank == handIn[4].rank) return true;
	else if(handIn[0].rank == handIn[1].rank&&
			handIn[2].rank == handIn[3].rank&&
			handIn[2].rank == handIn[4].rank) return true;
	else return false;
}

bool PokerHand::twoPair(vector<Card> &handIn)
{
	unsigned short int rankCount[15];
	for(int i=0;i<15;i++) rankCount[i] = 0;
	int pairCount = 0;
	for(int i=0;i<handIn.size();i++) rankCount[handIn[i].rank] += 1;
	for(int i=2;i<15;i++) if(rankCount[i]==2) pairCount++;
	
	if(pairCount==2) return true;
	else return false;
}

bool PokerHand::pair(vector<Card> &handIn)
{
	unsigned short int rankCount[15];
	for(int i=0;i<15;i++) rankCount[i] = 0;
	int pairCount = 0;
	for(int i=0;i<handIn.size();i++) rankCount[handIn[i].rank] +=1;
	for(int i=2;i<15;i++) if(rankCount[i]==2) pairCount++;

	if(pairCount==1) return true;
	else return false;
}

bool compareRank(Card a, Card b)
{
	if(a.rank<b.rank) return true;
	else return false;
}

void PokerHand::sortHandByRank(vector<Card> &handIn)
{
	std::sort(handIn.begin(),handIn.end());
}

bool PokerHand::compareStraightFlush(vector<Card> &handIn)
{
	if(handIn[4].rank==14) return false;
	else if(bestFive[4].rank==14) return true;
	else if(handIn[4].rank>bestFive[4].rank) return true;
	else return false;
}

bool PokerHand::compareFourOfAKind(vector<Card> &handIn)
{
	//Determine handIn four of kind and kicker
	int rankIn_FourOfKind;
	int rankIn_Kicker;
	if(handIn[0].rank == handIn[1].rank)
	{
		rankIn_FourOfKind = handIn[0].rank;
		rankIn_Kicker = handIn[4].rank;
	}
	else
	{
		rankIn_FourOfKind = handIn[4].rank;
		rankIn_Kicker = handIn[0].rank;
	}

	//Determine bestFive four of kind and kicker
	int bestFive_FourOfKind;
	int bestFive_Kicker;
	if(bestFive[0].rank == bestFive[1].rank)
	{
		bestFive_FourOfKind = bestFive[0].rank;
		bestFive_Kicker = bestFive[4].rank;
	}
	else
	{
		bestFive_FourOfKind = bestFive[4].rank;
		bestFive_Kicker = bestFive[0].rank;
	}

	//Compare four of kind (and kicker if tie)
	if(rankIn_FourOfKind>bestFive_FourOfKind) return true;
	else if(rankIn_FourOfKind==bestFive_FourOfKind)
	{
		if(rankIn_Kicker>bestFive_Kicker) return true;
		else return false;
	}
	else return false;
}

bool PokerHand::compareFullHouse(vector<Card> &handIn)
{
	//Determine handIn three of kind and pair
	int rankIn_ThreeOfKind;
	int rankIn_Pair;
	if( handIn[0].rank == handIn[1].rank&&
		handIn[0].rank == handIn[2].rank)
	{
		rankIn_ThreeOfKind = handIn[0].rank;
		rankIn_Pair = handIn[4].rank;
	}
	else
	{
		rankIn_ThreeOfKind = handIn[4].rank;
		rankIn_Pair = handIn[0].rank;
	}

	//Determine bestFive three of kind and pair
	int bestFive_ThreeOfKind;
	int bestFive_Pair;
	if( bestFive[0].rank == bestFive[1].rank&&
		bestFive[0].rank == bestFive[2].rank)
	{
		bestFive_ThreeOfKind = bestFive[0].rank;
		bestFive_Pair = bestFive[4].rank;
	}
	else
	{
		bestFive_ThreeOfKind = bestFive[4].rank;
		bestFive_Pair = bestFive[0].rank;
	}

	//Compare three of kinds (and pairs if tie)
	if(rankIn_ThreeOfKind>bestFive_ThreeOfKind) return true;
	else if(rankIn_ThreeOfKind==bestFive_ThreeOfKind)
	{
		if(rankIn_Pair>bestFive_Pair) return true;
		else return false;
	}
	else return false;
}

bool PokerHand::compareFlush(vector<Card> &handIn)
{
	if(handIn[4].rank==14) return false;
	else if(bestFive[4].rank==14) return true;
	else if(handIn[4].rank>bestFive[4].rank) return true;
	else if(handIn[3].rank>bestFive[3].rank) return true;
	else if(handIn[2].rank>bestFive[2].rank) return true;
	else if(handIn[1].rank>bestFive[1].rank) return true;
	else if(handIn[0].rank>bestFive[0].rank) return true;
	else return false;
}

bool PokerHand::compareStraight(vector<Card> &handIn)
{
	if(handIn[4].rank==14) return false;
	else if(bestFive[4].rank==14) return true;
	if(handIn[4].rank>bestFive[4].rank) return true;
	else if(handIn[3].rank>bestFive[3].rank) return true;
	else if(handIn[2].rank>bestFive[2].rank) return true;
	else if(handIn[1].rank>bestFive[1].rank) return true;
	else if(handIn[0].rank>bestFive[0].rank) return true;
	else return false;
}

bool PokerHand::compareThreeOfAKind(vector<Card> &handIn)
{
	//Determine rankIn 3 of kind, high card and kicker
	int rankIn_ThreeOfKind;
	int rankIn_HighCard;
	int rankIn_Kicker;
	//3 of kind is lowest rank
	if( handIn[0].rank == handIn[1].rank&&
		handIn[0].rank == handIn[2].rank)
	{
		rankIn_ThreeOfKind = handIn[0].rank;
		if(handIn[3].rank>handIn[4].rank)
		{
			rankIn_HighCard = handIn[3].rank;
			rankIn_Kicker = handIn[4].rank;
		}
		else if(handIn[4].rank>handIn[3].rank)
		{
			rankIn_HighCard = handIn[4].rank;
			rankIn_Kicker = handIn[3].rank;
		}
		else if(handIn[4].rank==handIn[3].rank)
		{
			rankIn_HighCard = handIn[3].rank;
			rankIn_Kicker = handIn[3].rank;
		}
	}
	//3 of kind is middle rank
	else if( handIn[1].rank == handIn[2].rank&&
		handIn[1].rank == handIn[3].rank)
	{
		rankIn_ThreeOfKind = handIn[1].rank;
		if(handIn[0].rank>handIn[4].rank)
		{
			rankIn_HighCard = handIn[0].rank;
			rankIn_Kicker = handIn[4].rank;
		}
		else if(handIn[4].rank>handIn[0].rank)
		{
			rankIn_HighCard = handIn[4].rank;
			rankIn_Kicker = handIn[0].rank;
		}
		else if(handIn[4].rank==handIn[0].rank)
		{
			rankIn_HighCard = handIn[0].rank;
			rankIn_Kicker = handIn[0].rank;
		}
	}
		//3 of kind is middle rank
	else
	{
		rankIn_ThreeOfKind = handIn[4].rank;
		if(handIn[0].rank>handIn[1].rank)
		{
			rankIn_HighCard = handIn[0].rank;
			rankIn_Kicker = handIn[1].rank;
		}
		else if(handIn[1].rank>handIn[0].rank)
		{
			rankIn_HighCard = handIn[1].rank;
			rankIn_Kicker = handIn[0].rank;
		}
		else if(handIn[1].rank==handIn[0].rank)
		{
			rankIn_HighCard = handIn[0].rank;
			rankIn_Kicker = handIn[0].rank;
		}
	}

	//Determine bestFive 3 of kind, high card and kicker
	int bestFive_ThreeOfKind;
	int bestFive_HighCard;
	int bestFive_Kicker;
	//3 of kind is lowest rank
	if( bestFive[0].rank == bestFive[1].rank&&
		bestFive[0].rank == bestFive[2].rank)
	{
		bestFive_ThreeOfKind = bestFive[0].rank;
		if(bestFive[3].rank>bestFive[4].rank)
		{
			bestFive_HighCard = bestFive[3].rank;
			bestFive_Kicker = bestFive[4].rank;
		}
		else if(bestFive[4].rank>bestFive[3].rank)
		{
			bestFive_HighCard = bestFive[4].rank;
			bestFive_Kicker = bestFive[3].rank;
		}
		else if(bestFive[4].rank==bestFive[3].rank)
		{
			bestFive_HighCard = bestFive[3].rank;
			bestFive_Kicker = bestFive[3].rank;
		}
	}
	//3 of kind is middle rank
	else if( bestFive[1].rank == bestFive[2].rank&&
		bestFive[1].rank == bestFive[3].rank)
	{
		bestFive_ThreeOfKind = bestFive[1].rank;
		if(bestFive[0].rank>bestFive[4].rank)
		{
			bestFive_HighCard = bestFive[0].rank;
			bestFive_Kicker = bestFive[4].rank;
		}
		else if(bestFive[4].rank>bestFive[0].rank)
		{
			bestFive_HighCard = bestFive[4].rank;
			bestFive_Kicker = bestFive[0].rank;
		}
		else if(bestFive[4].rank==bestFive[0].rank)
		{
			bestFive_HighCard = bestFive[0].rank;
			bestFive_Kicker = bestFive[0].rank;
		}
	}
		//3 of kind is middle rank
	else
	{
		bestFive_ThreeOfKind = bestFive[4].rank;
		if(bestFive[0].rank>bestFive[1].rank)
		{
			bestFive_HighCard = bestFive[0].rank;
			bestFive_Kicker = bestFive[1].rank;
		}
		else if(bestFive[1].rank>bestFive[0].rank)
		{
			bestFive_HighCard = bestFive[1].rank;
			bestFive_Kicker = bestFive[0].rank;
		}
		else if(bestFive[1].rank==bestFive[0].rank)
		{
			bestFive_HighCard = bestFive[0].rank;
			bestFive_Kicker = bestFive[0].rank;
		}
	}

	//Determine which is better 3 of kind
	if(rankIn_ThreeOfKind>bestFive_ThreeOfKind) return true;
	else if(rankIn_ThreeOfKind==bestFive_ThreeOfKind)
	{
		if(rankIn_HighCard>bestFive_HighCard) return true;
		else if(rankIn_Kicker>bestFive_Kicker) return true;
		else return false;
	}
	else return false;
}

bool PokerHand::compareTwoPair(vector<Card> &handIn)
{
	//Determine rankIn 2 pair ranks and high card
	int rankIn_HighPair = 0;
	int rankIn_LowPair = 0;
	int rankIn_HighCard = 0;

	unsigned short int rankCount[15];
	for(int i=0;i<15;i++) rankCount[i] = 0;

	for(int i=0;i<handIn.size();i++) rankCount[handIn[i].rank] += 1;

	for(int i=14;i>=2;i--)
	{
		if(rankCount[i]==2&&rankIn_HighPair==0) rankIn_HighPair = i;
		else if(rankCount[i]==2&&rankIn_HighPair!=0) rankIn_LowPair = i;
		else if(rankCount[i]==1) rankIn_HighCard = i;
	}

	//Determine bestfive 2 pair ranks and high card
	int bestFive_HighPair = 0;
	int bestFive_LowPair = 0;
	int bestFive_HighCard = 0;

	for(int i=0;i<15;i++) rankCount[i] = 0;

	for(int i=0;i<bestFive.size();i++) rankCount[bestFive[i].rank] += 1;

	for(int i=14;i>=2;i--)
	{
		if(rankCount[i]==2&&bestFive_HighPair==0) bestFive_HighPair = i;
		else if(rankCount[i]==2&&bestFive_HighPair!=0) bestFive_LowPair = i;
		else if(rankCount[i]==1) bestFive_HighCard = i;
	}

	//Compare rankIn and bestFive
	if(rankIn_HighPair>bestFive_HighPair) return true;
	else if(rankIn_HighPair==bestFive_HighPair)
	{
		if(rankIn_LowPair>bestFive_LowPair) return true;
		else if(rankIn_HighCard>bestFive_HighCard) return true;
		else return false;
	}
	else return false;

}

bool PokerHand::comparePair(vector<Card> &handIn)
{
	//Determine rankIn pair, high cards and kickers
	int rankIn_Pair = 0;
	int rankIn_HighCard = 0;
	int rankIn_Kicker = 0;
	int rankIn_KickerTwo = 0;

	unsigned short int rankCount[15];
	for(int i=0;i<15;i++) rankCount[i] = 0;
	for(int i=0;i<handIn.size();i++) rankCount[handIn[i].rank] += 1;
	for(int i=14;i>=2;i--)
	{
		if(rankCount[i]==2) rankIn_Pair = i;
		else if(rankCount[i]==1&&rankIn_HighCard==0) rankIn_HighCard = i;
		else if(rankCount[i]==1&&rankIn_HighCard>0&&rankIn_Kicker==0) rankIn_Kicker = i;
		else if(rankCount[i]==1&&rankIn_HighCard>0&&rankIn_Kicker>0&&rankIn_KickerTwo==0) rankIn_KickerTwo = i;
	}

	//Determine bestFive pair, high cards and kickers
	int bestFive_Pair = 0;
	int bestFive_HighCard = 0;
	int bestFive_Kicker = 0;
	int bestFive_KickerTwo = 0;

	for(int i=0;i<15;i++) rankCount[i] =0;
	for(int i=0;i<bestFive.size();i++) rankCount[bestFive[i].rank] += 1;
	for(int i=14;i>=2;i--)
	{
		if(rankCount[i]==2) bestFive_Pair = i;
		else if(rankCount[i]==1&&bestFive_HighCard==0) bestFive_HighCard = i;
		else if(rankCount[i]==1&&bestFive_HighCard>0&&bestFive_Kicker==0) bestFive_Kicker = i;
		else if(rankCount[i]==1&&bestFive_HighCard>0&&bestFive_Kicker>0&&bestFive_KickerTwo==0) bestFive_KickerTwo = i;
	}

	//Compare rankIn and bestFive
	if(rankIn_Pair>bestFive_Pair) return true;
	else if(rankIn_Pair==bestFive_Pair)
	{
		if(rankIn_HighCard>bestFive_HighCard) return true;
		else if(rankIn_Kicker>bestFive_Kicker) return true;
		else if(rankIn_KickerTwo>bestFive_KickerTwo) return true;
		else return false;
	}
	else return false;
}

void PokerHand::calcHandRank()
{
	for(int i=0;i<hand.size();i++)
	{
		for(int j=i+1;j<hand.size();j++)
		{
			for(int k=j+1;k<hand.size();k++)
			{
				for(int l=k+1;l<hand.size();l++)
				{
					for(int m = l+1;m<hand.size();m++)
					{
						vector<Card> tempHand;
						tempHand.push_back(hand[i]);
						tempHand.push_back(hand[j]);
						tempHand.push_back(hand[k]);
						tempHand.push_back(hand[l]);
						tempHand.push_back(hand[m]);
						sortHandByRank(tempHand);
						//cout << tempHand[0].rank << "  " << tempHand[1].rank << "  " << tempHand[2].rank << "  " << tempHand[3].rank << "  " << tempHand[4].rank << endl;

						//Royal Flush			-Not possible for multiple Royal Flushes
						if(royalFlush(tempHand)&&handRank<10)
						{
								handRank = 10;
								bestFive = tempHand;
						}

						//Straight Flush
						else if(straightDetected(tempHand)&&flushDetected(tempHand))
						{
							if(handRank<9)
							{
								handRank = 9;
								bestFive = tempHand;
							}
							else if(handRank==9)
							{
								if(compareStraightFlush(tempHand)) bestFive = tempHand;
							}
						}

						//Four of a Kind		
						else if(fourOfAKind(tempHand))
						{
							if(handRank<8)
							{
								handRank = 8;
								bestFive = tempHand;
							}
							else if(handRank==8)
							{
								if(compareFourOfAKind(tempHand)) bestFive = tempHand;
							}

						}

						//Full House
						else if(fullHouse(tempHand))
						{
							if(handRank<7)
							{
								handRank = 7;
								bestFive = tempHand;
							}
							else if(handRank==7)
							{
								if(compareFullHouse(tempHand)) bestFive = tempHand;
							}
						}

						//Flush
						else if(flushDetected(tempHand))
						{
							if(handRank<6)
							{
								handRank = 6;
								bestFive = tempHand;
							}
							else if(handRank==6)
							{
								if(compareFlush(tempHand)) bestFive = tempHand;
							}
						}

						//Straight
						else if(straightDetected(tempHand))
						{
							if(handRank<5)
							{
								handRank = 5;
								bestFive = tempHand;
							}
							else if(handRank==5)
							{
								if(compareStraight(tempHand)) bestFive = tempHand;
							}
						}

						//Three of a Kind
						else if(threeOfAKind(tempHand))
						{
							if(handRank<4)
							{
								handRank = 4;
								bestFive = tempHand;
							}
							else if(handRank==4)
							{
								if(compareThreeOfAKind(tempHand)) bestFive = tempHand;
							}
						}

						//Two Pair
						else if(twoPair(tempHand))
						{
							if(handRank<3)
							{
								handRank = 3;
								bestFive = tempHand;
							}
							else if(handRank==3)
							{
								if(compareTwoPair(tempHand)) bestFive = tempHand;
							}
						}

						//Pair
						else if(pair(tempHand))
						{
							if(handRank<2)
							{
								handRank = 2;
								bestFive = tempHand;
							}
							else if(handRank==2)
							{
								if(comparePair(tempHand)) bestFive = tempHand;
							}
						}

						//High Card
						else
						{
							if(handRank<1) handRank = 1;
							if(hand[0].rank>hand[1].rank)
							{
								highCard = hand[0].rank;
								kicker = hand[1].rank;
							}
							else if(hand[1].rank>hand[0].rank)
							{
								highCard = hand[1].rank;
								kicker = hand[0].rank;
							}
							else
							{
								highCard = hand[1].rank;
								kicker = hand[1].rank;
							}
						}
					}
				}
			}
		}
	}
}

void PokerHand::printBestHand()
{
	cout << "          ";
	if(handRank==10) cout << "Royal Flush\n";
	else if(handRank==9) cout << "Straight Flush\n";
	else if(handRank==8) cout << "Four of a Kind\n";
	else if(handRank==7) cout << "Full House\n";
	else if(handRank==6) cout << "Flush\n";
	else if(handRank==5) cout << "Straight\n";
	else if(handRank==4) cout << "Three of a Kind\n";
	else if(handRank==3) cout << "Two Pair\n";
	else if(handRank==2) cout << "One Pair\n";
	else if(handRank==1) cout << "High Card: " << highCard << "  Kicker: " << kicker << endl;
	else if(handRank==0) cout << "HAND NOT PROGRAMMED YET (or correctly)\n";

	if(bestFive.size()!=5) cout << "bestFive incorrect size!!   " << bestFive.size() << endl;
	for(int i=0;i<bestFive.size();i++)
	{
		bestFive[i].printCard();
	}
}

bool PokerHand::operator==(const PokerHand &other)
{
	if(handRank==1&&other.handRank==1)
	{
		if(highCard==other.highCard&&kicker==other.kicker) return true;
		else return false;
	}
	else if(handRank==other.handRank&&
	   bestFive[0].rank==other.bestFive[0].rank&&
	   bestFive[1].rank==other.bestFive[1].rank&&
	   bestFive[2].rank==other.bestFive[2].rank&&
	   bestFive[3].rank==other.bestFive[3].rank&&
	   bestFive[4].rank==other.bestFive[4].rank) return true;
	else return false;
}

bool PokerHand::operator<(const PokerHand &other)
{
	cout << "1\n";
	//if(*this==other) return false;
	vector<Card> tempHand = other.bestFive;
	if(handRank>other.handRank) return false;
	else if(other.handRank>handRank) return true;
	if(handRank==other.handRank)
	{
		switch(handRank)
		{
		case 9:
			if(compareStraightFlush(tempHand)) return true;
			else return false;
			break;
		case 8:
			if(compareFourOfAKind(tempHand)) return true;
			else return false;
			break;
		case 7:
			if(compareFullHouse(tempHand)) return true;
			else return false;
			break;
		case 6:
			if(compareFlush(tempHand)) return true;
			else return false;
			break;
		case 5:
			if(compareStraight(tempHand)) return true;
			else return false;
			break;
		case 4:
			if(compareThreeOfAKind(tempHand)) return true;
			else return false;
			break;
		case 3:
			if(compareTwoPair(tempHand)) return true;
			else return false;
			break;
		case 2:
			if(comparePair(tempHand)) return true;
			else return false;
			break;
		case 1:
			if(highCard>other.highCard) return false;
			else if(highCard==other.highCard)
			{
				if(kicker>other.kicker) return false;
				else return true;
			}
			else return true;
			break;
		}
		return false;
	}
	else return false;
}

void PokerHand::resetPokerHand()
{
	hand.clear();
	bestFive.clear();
	handRank = 0;
	highCard = 0;
	kicker = 0;
}

