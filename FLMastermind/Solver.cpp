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
	if (current.size() == lengthOfCode) {
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
	for (int i = 0; i < lengthOfCode; i++) {
		if (guess[i] == answer[i]) {
			fb.exact++;
		};
	};

	// count partial
	for (int i = 0; i < numColors; i++) {
		int countGuess = 0;
		int countAnswer = 0;

		for (int j = 0; j < lengthOfCode; j++) {
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

// returns depth of decision tree
int Solver::maxDepth(TreeNode* node) {
	// early return cases
	if (node == nullptr) {
		return 0;
	};
	if (node->children.size() == 0) {
		return 1;
	};

	int deepest = 0;

	// recursively find the deepest child
	for (int i = 0; i < node->children.size(); i++) {
		int childDepth = maxDepth(node->children.at(i));
		if (childDepth > deepest) {
			deepest = childDepth;
		};
	};
	return deepest + 1; // add 1 to include current node
};

// check if current strategy (mainly greedy) is good enough
bool Solver::isOptimal(TreeNode* node, int depthLimit) {
	// tree is empty (early return)
	if (node == nullptr) {
		return true;
	};

	// if we reached depthLimit but still have more children this would not be optimal
	if (depthLimit == 0 && node->children.size() > 0) {
		return false;
	};

	// recursively checks all children
	for (int i = 0; i < node->children.size(); i++) {
		if (!isOptimal(node->children.at(i), depthLimit - 1)) {
			return false;
		};
	};

	return true;
};


// ---------- Main Backtracking Function ----------
bool Solver::backTrack(TreeNode* node, Vector<std::string>& remaining, int depth) {
	// early return cases
	if (remaining.size() == 0) { // no possible codes remaining
		return false;
	};

	if (remaining.size() == 1) { // only one code left
		node->guess = remaining.at(0);
		return true;
	};

	if (depth == 0) { // depth limit was reached
		return false;
	};

	for (int i = 0; i < remaining.size(); i++) {
		string candidate = remaining.at(i);
		node->guess = candidate;

		// split codes into buckets, kind of what we did in greedy
		Vector<GuessFeedback> fbs(14);
		Vector<Vector<string>> buckets(14);
		buildBuckets(candidate, remaining, fbs, buckets);

		bool success = true;

		// check each possible feedback
		for (int j = 0; j < fbs.size(); j++) {

			// all positions are correct (skip)
			if (fbs.at(j).exact == lengthOfCode) {
				continue;
			};

			// pruning, abandon a guess that does not reduce the search
			if (buckets.at(j).size() == remaining.size()) {
				success = false;
				break;
			}

			TreeNode* child = new TreeNode;
			child->feedback = fbs.at(j);

			// recursively solve
			if (!backTrack(child, buckets.at(j), depth - 1)) {
				success = false;
				delete child;
				break;
			};

			// store successful branch
			node->children.push_back(child);
		};

		// if branches succeed it was a valid strategy
		if (success) {
			return true;
		};

		// undo all children created for this candidate
		for (int j = 0; j < node->children.size(); j++) {
			delete node->children.at(j);
		};

		node->children.clear();
	};
	return false; // failed case
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
		if (fbs.at(i).exact == lengthOfCode) {
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

int Solver::intInputCheck(int min, int max) {
	int x;

	while (true) {
		cin >> x;

		// checks for letters and special characters
		if (cin.fail()) {
			cout << "Invalid input. Enter a number: ";
			cin.clear();
			cin.ignore(10000, '\n');
		}
		// checks if input is greater than 4 or less than 0
		else if (x < min || x > max) {
			cout << "Invalid input. Enter a number between " << min << " and " << max << ": ";
		}
		else {
			return x;
		};
	};
};

// ---------- UI/Main loop to solve a code ----------
// more of an interactive solve than I intended but I kind of like it
void Solver::solve() {
	cout << "\n--- Mastermind Solver ---" << endl;
	cout << "Think of a secret code using these colors: R G B Y U O" << endl;

	Vector<string> remaining = allCodes;
	const int btLimit = 50;

	// make fixed first guess before building tree
	string firstGuess = "RRGG";
	cout << "Move 1 - My guess: " << firstGuess << endl;

	// receive feedback from user about how many exact and partial matches there are
	int exact, partial;
	cout << "Enter exact matches: ";
	exact = intInputCheck(0, 4);
	cout << "Enter partial matches: ";
	partial = intInputCheck(0, 4);

	// check if everything was exact --> if it was the code was solved
	if (exact == lengthOfCode || exact == 3 && partial == 1) {
		if (partial == 1) {
			cout << "\nThe combination of 3 exact and 1 partial matches mean the code was solved!" << endl;
		};
		cout << "\nSolved! The code was " << firstGuess << "!" << endl;
		system("pause");
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
	cout << "\nBuilding greedy tree..." << endl;
	TreeNode* root = buildTree(remaining, lengthOfCode);
	tree.setRoot(root);
	cout << "Greedy tree built. Worst case moves: " << maxDepth(root) + 2 << endl << endl;

	// navigate tree for remaining guesses
	TreeNode* current = tree.getRoot();
	int moveCount = 2;

	// main loop
	while (current != nullptr) {
		cout << "Move " << moveCount++ << " - My guess: " << current->guess << endl;

		cout << "Enter exact matches: ";
		exact = intInputCheck(0, 4);
		cout << "Enter partial matches: ";
		partial = intInputCheck(0, 4);

		// special case where user entered matches wrong and ended up solving without realizing
		if (exact == lengthOfCode || exact == 3 && partial == 1) {
			if (partial == 1) {
				cout << "\nThe combination of 3 exact and 1 partial matches mean the code was solved!" << endl;
			};
			cout << "\nSolved! The code was " << current->guess << "!" << endl;
			system("pause");
			return;
		};

		// find correct part of the tree to go down based on the feedback from user
		TreeNode* next = nullptr;
		for (int i = 0; i < current->children.size(); i++) {
			if (current->children.at(i)->feedback.isEqual(GuessFeedback(exact, partial))) {
				next = current->children.at(i);
				break;
			};
		};

		// runs basically if there was user input error
		if (next == nullptr) {
			cout << "No matching path in tree. Check input." << endl;
			system("pause");
			return;
		};

		current = next;
		cout << "Current remaining codes: " << current->remaining.size() << endl;

		// recheck with backtracking at each step -- maybe it will actually be better at some point
		// this is because remaining codes shrinks each time and otherwise it would take like 20+ minutes to trigger
		if (current != nullptr && !isOptimal(current, lengthOfCode - 2)) {
			if (current->remaining.size() <= btLimit) {
				cout << "Checking with backtracking..." << endl;
				int currDepth = maxDepth(current);
				TreeNode* btNode = new TreeNode();
				if (backTrack(btNode, current->remaining, lengthOfCode)) {
					int btDepth = maxDepth(btNode);
					if (btDepth < currDepth) {
						cout << "Backtracking improved subtree. Worst case moves: " << btDepth << endl;
						btNode->feedback = current->feedback;
						current = btNode;
					}
					else {
						cout << "Backtracking did not improve. Using Greedy." << endl;
						delete btNode;
					};
				}
				else {
					delete btNode;
				};
			};
		};
	};
	cout << "No moves available." << endl; // solver completely failed
	system("pause");
};


// ---------- testing ----------
void Solver::test() {
	cout << "Solver tests" << endl;
	cout << "------------------------" << endl;
	//testGetFeedback();
	//testGenerateCodes();
	testBacktracking();
	system("pause");
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
				noDups = false;
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

void Solver::testBacktracking() {
	cout << "\n--- Backtracking Tests --- " << endl;


	// testing small remaining number of codes with backtracking
	Vector<string> small;
	small.push_back("RRYY");
	small.push_back("RRBU");
	small.push_back("RRBB");
	small.push_back("RRBO");
	small.push_back("RRBY");
	TreeNode* btNode1 = new TreeNode();
	bool found1 = backTrack(btNode1, small, lengthOfCode);
	if (found1 == true) {
		cout << "Backtracking found a solution for small number of remaining codes." << endl;
	}
	else {
		cout << "Backtracking did not find solution for small number of codes" << endl;
	};
	if (btNode1->guess != "") {
		cout << "Backtracking has a guess: " << btNode1->guess << endl;
	}
	else {
		cout << "Backtracking has no guess" << endl;
	};
	if (btNode1->children.size() > 0) {
		cout << "Backtracking tree has children" << endl;
	}
	else {
		cout << "Backtracking tree has no children" << endl;
	};
	delete btNode1;


	// testing backtracking when there is no codes remaining
	Vector<string> empty;
	TreeNode* btNode2 = new TreeNode();
	bool found2 = backTrack(btNode2, empty, lengthOfCode);
	if (found2 == false) {
		cout << "Backtracking returned false for no codes (good)" << endl;
	}
	else {
		cout << "Backtracking failed when no codes remained (bad)" << endl;
	};
	delete btNode2;

	
	// testing backtracking when one code remains
	Vector<string> oneCode(1);
	oneCode.push_back("RGBY");
	TreeNode* btNode3 = new TreeNode();
	backTrack(btNode3, oneCode, lengthOfCode);
	if (btNode3->guess == "RGBY") {
		cout << "Backtracking correctly got the last code" << endl;
	}
	else {
		cout << "Backtracking did not get the last code" << endl;
	};
	delete btNode3;


	// testing backtracking when depth is 0
	Vector<string> depth0;
	depth0.push_back("RGBY");
	depth0.push_back("RRRR");
	TreeNode* btNode4 = new TreeNode();
	bool found4 = backTrack(btNode4, depth0, 0);
	if (found4 == false) {
		cout << "Backtracking returned false when depth was 0 (good)" << endl;
	}
	else {
		cout << "Backtracking failed when depth was 0" << endl;
	};
	delete btNode4;
};