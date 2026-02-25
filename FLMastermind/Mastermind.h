#ifndef MASTERMIND_H
#define MASTERMIND_H

#include <iostream>
#include "Sequence.h"

using namespace std;
class Mastermind
{
public:
	void gameChoice();
	void gamePlay();
	void printColors();
	bool choiceValidation(char c);
	bool validateColor(char c);
	void makeSequence();
	void printSecretCode();
	void getPlayerGuess();
	bool compareSequences();
private:
	Sequence code;
	Sequence guess;
};

#endif