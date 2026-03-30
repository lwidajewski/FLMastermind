#include <iostream>
#include <string>

#include "Solver.h"

using namespace std;

// ---------- Constructor ----------
Solver::Solver() {
	//cout << "Solver constructor called" << endl;
	generateCodes(allCodes, ""); // generate all 1296 codes
	//cout << "generate codes complete, allCodes size: " << allCodes.size() << endl;
};

// ---------- Code Generation ----------
// creates all 1296 codes using recursion (e.g. RRRR to OOOO and stuff to help with solving)
// could have done like 4 for loops but I didn't like the way it looked
void Solver::generateCodes(Vector<string>& codes, string current) {
	if (current.size() == length) {
		codes.push_back(current);
		return;
	}
	else {
		for (int i = 0; i < numColors; i++) {
			generateCodes(codes, current + colors[i]);
		};
	};
};

// ---------- Exact + Partial matches feedback ----------
// uses GuessFeedback struct declared in Tree to keep track of exact/partial matches
GuessFeedback Solver::getFeedback(std::string guess, std::string answer) {
	GuessFeedback fb; // stores # of exact and partial matches

	// count exact - length = 4 since that is how long the code being guessed is
	for (int i = 0; i < length; i++) {
		if (guess[i] == answer[i]) {
			fb.exact++;
		};
	};

	// count partial
	for (int i = 0; i < numColors; i++) {
		int countGuess = 0;
		int countAnswer = 0;

		for (int j = 0; j < length; j++) {
			if (guess[j] == colors[i]) {
				countGuess++;
			};
			if (answer[j] == colors[i]) {
				countAnswer++;
			};
		};
		if (countGuess < countAnswer) {
			fb.partial += countGuess;
		}
		else {
			fb.partial += countAnswer;
		};
	};

	// remove exact matches from partial matches (otherwise we are double counting)
	fb.partial = fb.partial - fb.exact;

	return fb;
};

// ---------- Bucket Creation ----------
// create buckets/containers using vectors to store different types of feedback
// e.g. 1 exact, 2 partial (1,2) | e.g. 2 exact, 0 partial (2,0)
void Solver::buildBuckets(const string& guess, Vector<string>& remaining, Vector<GuessFeedback>& seenFBs, Vector<Vector<string>>& buckets) {
	for (int i = 0; i < remaining.size(); i++) {
		//cout << "buildBuckets called, remaining=" << remaining.size() << endl;
		// get the partial and exact matches of the guess
		GuessFeedback fb = getFeedback(guess, remaining.at(i));

		bool foundBucket = false;

		// sort seen feedbacks into their respective bucket
		// e.g. we saw the feedback 2 exact, 1 partial so we put another feedback of the same 2,1 in the same bucket
		for (int j = 0; j < seenFBs.size(); j++) {
			if (seenFBs.at(j).isEqual(fb)) {
				buckets.at(j).push_back(remaining.at(i));
				foundBucket = true;
				break;
			};
		};

		// if we have not seen the feedback yet maybe 3 exact, 0 partial we create a new bucket to put those feedbacks in
		if (!foundBucket) {
			//cout << "New bucket, current bucket count: " << seenFBs.size() << endl;
			seenFBs.push_back(fb);
			Vector<string> newBucket(totalCodes);
			newBucket.push_back(remaining.at(i));
			buckets.push_back(newBucket);
		};
	};
};

// ---------- Greedy Approach Implementation ----------
// pick the guess that will minimize the worst case (largest bucket size), but still split the data well
string Solver::greedyPick(Vector<string>& remaining) {
	//cout << "greedyPick called, remaining=" << remaining.size() << endl;
	string best;
	int bestWorstCaseSize = allCodes.size(); // can't be more than the number of codes

	for (int i = 0; i < remaining.size(); i++) {
		string candidateGuess = remaining.at(i); // start

		// split remaining codes into buckets - testing every single remaining code against candidate
		// the 14 here just initializes the capacity, see Vector.h --> I think this could be less but I was testing a lot and changed a lot and it works fine
		Vector<GuessFeedback> fbs(14);
		Vector<Vector<string>> buckets(14);
		// build buckets comparing it to candidate
		buildBuckets(candidateGuess, remaining, fbs, buckets);

		// find size of worst case (largest bucket)
		int largeBucketSize = 0;
		for (int j = 0; j < buckets.size(); j++) {
			if (buckets.at(j).size() > largeBucketSize) {
				largeBucketSize = buckets.at(j).size();
			};
		};

		// pick the guess that splits the data the best (not necessarily the smallest bucket, I got confused here so I'll give an example)
		// when splitting up a group of 100 people you don't split them 1 and 99 you split them 50,50
		// here is the same thing but you might not be able to split it down the middle, but you try to pick where it would be most balanced.
		if (largeBucketSize < bestWorstCaseSize) {
			bestWorstCaseSize = largeBucketSize;
			best = candidateGuess;
		};
	};
	return best;
};

// ---------- Build Decision Tree ----------
// one parent can have multiple children (more than 2) --> this is because one guess can produce several different feedbacks
TreeNode* Solver::buildTree(Vector<string>& remaining, int depth) {
	//cout << "buildTree called, depth=" << depth << " remaining=" << remaining.size() << endl;
	// base case: nothing left, or depth limit was reached
	if (remaining.size() == 0 || depth == 0) {
		return nullptr;
	};

	TreeNode* node = new TreeNode();
	node->remaining = remaining; // all possible answers at this point --> also good for backtracking
	node->guess = greedyPick(remaining); // best guess using greedy approach

	// split everything up into buckets
	Vector<GuessFeedback> fbs(14);
	Vector<Vector<string>> buckets(14);
	buildBuckets(node->guess, remaining, fbs, buckets);

	// go through each feedback
	for (int i = 0; i < fbs.size(); i++) {
		// check for correct answer (4,0) --> 4 exact, 0 partial
		if (fbs.at(i).exact == length) {
			continue;
		};

		// build decision tree recursively
		TreeNode* child = buildTree(buckets.at(i), depth - 1);

		// attach the child node to the correct parent node
		if (child) {
			child->feedback = fbs.at(i);
			node->children.push_back(child);
		};
	};
	return node;
};

// ---------- UI/Main loop to solve a code ----------
// more of an interactive solve than I intended but I kind of like it
void Solver::solve() {
	cout << "\n--- Mastermind Solver ---" << endl;
	cout << "Think of a secret code using these colors: R G B Y U O" << endl;

	Vector<string> remaining = allCodes;

	// make fixed first guess before building tree
	string firstGuess = "RRGG";
	cout << "Move 1 - My guess: " << firstGuess << endl;

	// receive feedback from user about how many exact and partial matches there are
	int exact, partial;
	cout << "Enter exact matches: ";
	cin >> exact;
	cout << "Enter partial matches: ";
	cin >> partial;

	// check if everything was exact --> if it was the code was solved
	if (exact == length) {
		cout << "\nSolved! The code was " << firstGuess << "!" << endl;
		return;
	};

	// filter remaining based on first guess feedback
	Vector<string> filtered;
	for (int i = 0; i < remaining.size(); i++) {
		if (getFeedback(firstGuess, remaining.at(i)).isEqual(GuessFeedback(exact, partial))) {
			filtered.push_back(remaining.at(i));
		};
	};
	remaining = filtered;
	cout << "Remaining codes after filter: " << remaining.size() << endl;

	// error if there was a misinput
	if (remaining.size() == 0) {
		cout << "No remaining codes match feedback. Check input." << endl;
		return;
	};

	// build tree with remaining codes after first guess
	cout << "Building decision tree..." << endl;
	TreeNode* root = buildTree(remaining, length);
	tree.setRoot(root);
	cout << "Tree built! Let's solve." << endl;

	// navigate tree for remaining guesses
	TreeNode* current = tree.getRoot();
	int moveCount = 2;

	// main loop
	while (current != nullptr) {
		cout << "Move " << moveCount++ << " - My guess: " << current->guess << endl;

		cout << "Enter exact matches: ";
		cin >> exact;
		cout << "Enter partial matches: ";
		cin >> partial;

		if (exact == length) {
			cout << "\nSolved! The code was " << current->guess << "!" << endl;
			return;
		};

		TreeNode* next = nullptr;
		for (int i = 0; i < current->children.size(); i++) {
			if (current->children.at(i)->feedback.isEqual(GuessFeedback(exact, partial))) {
				next = current->children.at(i);
				break;
			};
		};

		if (next == nullptr) {
			cout << "No matching path in tree. Check input." << endl;
			return;
		};

		current = next;
	};

	cout << "No moves available." << endl;
};


// ---------- testing ----------
void Solver::test() {
	//testGetFeedback();
	//testGenerateCodes();
};

// yes I inputted codes manually to check
void Solver::testGetFeedback() {
	GuessFeedback fb;

	for (int i = 0; i < 15; i++) {
		string guess = "";
		string answer = "";

		cout << "\nEnter guess code: ";
		cin >> guess;
		cout << "Enter answer code: ";
		cin >> answer;
		fb = getFeedback(guess, answer);

		cout << "Exact: " << fb.exact << " Partial: " << fb.partial;
	};
};

// testing 1296 codes
void Solver::testGenerateCodes() {
	cout << "Generate codes creates 1296 codes. Total: " << allCodes.size() << endl;

	cout << "First code is RRRR, index 0: " << allCodes.at(0) << endl;

	cout << "Last code is OOOO, index 1295: " << allCodes.at(1295) << endl;

	bool isLength4 = true;
	for (int i = 0; i < allCodes.size(); i++) {
		if (allCodes.at(i).length() != 4) {
			isLength4 = false;
		};
	};
	if (isLength4 == true) {
		cout << "All codes are length of 4" << endl;
	}
	else {
		cout << "Not every code is length 4" << endl;
	};

	bool noDups = true;
	for (int i = 0; i < allCodes.size(); i++) {
		for (int j = i + 1; j < allCodes.size(); j++) {
			if (allCodes.at(i) == allCodes.at(j)) {
				bool noDups = false;
			};
		};
	};
	if (noDups == true) {
		cout << "No duplicates" << endl;
	}
	else {
		cout << "There are duplicates" << endl;
	};
};