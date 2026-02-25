#include "Mastermind.h"
#include "Sequence.h"

using namespace std;

void Mastermind::gameChoice() {		//user chooses either the game or MastermindSolver
	string choice;
	cout << "Would you like to use Mastermind Solver or play?(S or P): ";
	cin >> choice;
	while (choice.length() != 1 || !choiceValidation(toupper(choice[0])))
	{
		cout << "Invalid Response: (S or P):";
		cin >> choice;
	}
}

bool Mastermind::choiceValidation(char c) {
	switch (c) {
	case 'S':
		//temporary message
		cout << "Under Development";
		return true;
		break;
	case 'P':
		gamePlay();
		return true;
		break;
	default:
		return false;
	}
}

void Mastermind::gamePlay() {
	int turnNum = 0;
	int maxAttempts = 10;
	bool won = false;


	printColors();
	makeSequence();

	// start game loop
	printColors();
	while (!won && turnNum < maxAttempts) {
		cout << "\nAttempt #" << turnNum + 1 << endl;

		getPlayerGuess();
		won = compareSequences();

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
		printSecretCode();
	};
}

void Mastermind::printColors() {		//prints the possible colors for players to input
	cout << "The colors are: " << endl;
	cout << "R = Red" << endl;
	cout << "G = Green" << endl;
	cout << "B = Blue" << endl;
	cout << "Y = Yellow" << endl;
	cout << "U = Purple" << endl;
	cout << "W = White" << endl;
	cout << "L = Black" << endl;
	cout << "O = Orange" << endl;
	cout << "P = Pink\n" << endl;
};

bool Mastermind::validateColor(char c) {
	// checks input to see if user entered a valid color
	switch (c) {
	case 'R': case 'G': case 'B': case 'Y': case 'U': case 'W': case 'L': case 'O': case 'P':
		return true;
	default:
		return false;
	};
};

void Mastermind::makeSequence() {			//asks user to input a sequence
	string input;

	// clear the code list to put values in it (maybe in the future we add functionality to continue playing)
	code.clear();

	for (int i = 1; i <= 4; i++) {
		// get user input
		cout << "\nPlayer 1: enter color " << i << " of the code: ";
		cin >> input;

		// validate input
		while (input.length() != 1 || !validateColor(toupper(input[0]))) {
			cout << "Invalid input. Please enter ONE color (R,G,B,Y,U,W,L,O,P): ";
			cin >> input;
		};

		// convert first character in string to uppercase
		char c = toupper(input[0]);

		// insert into code linkedlist
		if (i == 1) {
			code.insertAtStart(c);
		}
		else {
			code.insertAtEnd(c);
		};
	};
	// print out the initial secret code
	cout << "The sequence is: ";
	code.print();

	//pauses program then clears console so player 2 can't see
	system("pause");
	system("CLS");
};

// print out the secret code if player lost
void Mastermind::printSecretCode() {
	code.print();
};

void Mastermind::getPlayerGuess() {
	string input;

	// clear the guess list to put more values in it
	guess.clear();

	for (int i = 1; i <= 4; i++) {
		cout << "Player 2: enter color " << i << ": " << endl;
		cin >> input;

		// input validation
		while (input.length() != 1 || !validateColor(toupper(input[0]))) {
			cout << "Invalid input. Please enter a color (R,G,B,Y,U,W,L,O,P): ";
			cin >> input;
		};

		// convert first character in string to uppercase
		char c = toupper(input[0]);

		// insert player guess into linkedlist
		if (i == 1) {
			guess.insertAtStart(c);
		}
		else {
			guess.insertAtEnd(c);
		};
	};

	// print out the player's guess
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