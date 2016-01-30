#include "Pot.h"


Pot::Pot(string nameIn)
{
	ammount =0;
	name = nameIn;
}

Pot::~Pot(void)
{
}

void Pot::addToPot(int ammountIn)
{
	ammount += ammountIn;
}

void Pot::printPot()
{
	cout << "     " << name << ": " << ammount << endl;
}
