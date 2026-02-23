#include "Mastermind.h"
#include "Sequence.h"


void Mastermind::printColors() {		//prints the possible colors for players to input
	cout << "The colors are: 1,2,3,4,5,6,7,8,9" << endl;
}

void Mastermind::makeSequence() {			//asks user to input a sequence
	char c;

	cout << "Player 1: please input first color" << endl;
	cin >> c;
	code.insertAtStart(c);
	cout << "Player 1: please input second color" << endl;
	cin >> c;
	code.insertAtEnd(c);
	cout << "Player 1: please input third color" << endl;
	cin >> c;
	code.insertAtEnd(c);
	cout << "Player 1: please input fourth color" << endl;
	cin >> c;
	code.insertAtEnd(c);

	cout << "The sequence is: ";
	code.print();
	system("pause");
}

void Mastermind::getPlayerGuess() {
	char c;

	// sets up an empty list to put the guesses in
	// also clears any previous guesses that was in the list because of the destructor
	guess = Sequence();

	for (int i = 1; i <= 4; i++) {
		cout << "Player 2: enter color " << i << ": " << endl;
		cin >> c;
		guess.insertAtEnd(c);
	};

	cout << "Player 2's guess: ";
	guess.print();
};

// returns true if player wins with an exact match of 4 colors
// returns false if player has not gotten an exact match yet
bool Mastermind::compareSequences() {
	// get the start of the lists
	Node* codePtr = code.getHead();
	Node* guessPtr = guess.getHead();

	// check for empty sequences first
	if (codePtr != nullptr && guessPtr != nullptr) {

		// initialize arrays
		char codeArr[4];
		char guessArr[4];

		// put the secret code and the player guess into the arrays from the linked list
		for (int i = 0; i < 4; i++) {
			codeArr[i] = codePtr->color;
			guessArr[i] = guessPtr->color;

			codePtr = codePtr->next;
			guessPtr = guessPtr->next;
		};

		// intialize counters
		int exactMatch = 0;
		int partialMatch = 0;

		// these are like checklist arrays so we aren't double checking items when comparing
		bool codeUsed[4] = { false };
		bool guessUsed[4] = { false };

		// count the exact matches
		for (int i = 0; i < 4; i++) {
			if (codeArr[i] == guessArr[i]) {
				exactMatch++;
				codeUsed[i] = true;
				guessUsed[i] = true;
			};
		};

		// count the partial matches (and skipping over the ones we have already counted as exact matches)
		for (int i = 0; i < 4; i++) {
			// if the guess is still false (no exact matches at this location) continue
			if (!guessUsed[i]) {
				for (int j = 0; j < 4; j++) {
					// if the code array location is not true check to see if it equals the users guess
					// this prevents checking the same location multiple times if the location already had a partial match
					if (!codeUsed[j] && guessArr[i] == codeArr[j]) {
						partialMatch++;
						codeUsed[j] = true;
						break;
					};
				};
			};
		};

		// check if player won
		if (exactMatch == 4) {
			// player has won
			return true;
		}
		else {
			// print output/feedback if they have not won
			cout << "Exact matches: " << exactMatch << endl;
			cout << "Partial matches: " << partialMatch << endl;
			return false;
		};
	}
	else {
		cout << "Sequences are empty" << endl;
	};
};