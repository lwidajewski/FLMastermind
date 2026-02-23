#ifndef MASTERMIND_H
#define MASTERMIND_H

#include <iostream>
#include "Sequence.h"

using namespace std;
class Mastermind
{
public:
	void printColors();
	void makeSequence();
	void printSecretCode();
	void getPlayerGuess();
	bool compareSequences();
private:
	Sequence code;
	Sequence guess;
};

#endif