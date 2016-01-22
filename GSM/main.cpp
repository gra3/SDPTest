#include <iostream>
#include <stdio.h>
#include <concrt.h>
#include "Card.h"
#include "CardDetect.h"
#include "Player.h"
#include "Game.h"
#include "Deck.h"

using namespace std;

int main(int argc, char* args[])
{
	Game game(3, 0.25, 0.5, 20.0);
	game.start();

	return 0;
}