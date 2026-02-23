#include <iostream>

#include "Mastermind.h"
using namespace std;

int main() {
	cout << "\nFinn and Leo's Mastermind Game" << endl;
	cout << "-----------------------------------------------------------------------" << endl << endl;

	Mastermind game;
	int turnNum = 0;
	int maxAttempts = 10;
	bool won = false;

	game.printColors();
	game.makeSequence();

	// start game loop
	while (!won && turnNum < maxAttempts) {
		cout << "\nAttempt #" << turnNum + 1 << endl;

		game.getPlayerGuess();
		won = game.compareSequences();

		turnNum++;
	};

	if (won) {
		// player won print out win screen
		cout << "\nYou have cracked the code and won!" << endl;
	}
	else {
		// player lost via reaching max attempts
		cout << "\nYou have lost! You've used all your attempts!" << endl;
		// print out the secret code that was not guessed by the player
		game.printSecretCode();
	};
};
