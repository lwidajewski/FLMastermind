#include <iostream>

#include "Mastermind.h"
using namespace std;

int main() {
	cout << "\nFinn and Leo's Mastermind Game" << endl;
	cout << "-----------------------------------------------------------------------" << endl << endl;

	Mastermind game;

	game.printColors();
	game.makeSequence();
	game.getPlayerGuess();
};
