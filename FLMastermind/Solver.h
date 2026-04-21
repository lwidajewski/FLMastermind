#ifndef SOLVER_H
#define SOLVER_H

#include <string>

#include "Tree.h"
#include "Vector.h"

class Solver {
	private:
		const int lengthOfCode = 4; // length of a code e.g. RRRR
		const int numColors = 6;
		const int totalCodes = 1296;
		const char colors[6] = { 'R', 'G', 'B', 'Y', 'U', 'O' }; // colors that can be guessed for a code
		Vector<string> allCodes; // stores every code
		Tree tree; // tree initialization

		// creates all 1296 codes
		void generateCodes(Vector<std::string>& codes, std::string current);

		// returns # of partial and exact matches as a struct
		GuessFeedback getFeedback(std::string guess, std::string answer);

		// builds vectors that are stored in a vector-->called buckets here (vectorception)
		void buildBuckets(const string& guess, Vector<string>& remaining, Vector<GuessFeedback>& seenFBs, Vector<Vector<string>>& buckets);

		// greedy approach implementation --> see solver.cpp for more explanation
		std::string	greedyPick(Vector<std::string>& remaining);

		bool backTrack(TreeNode* node, Vector<std::string>& remaining, int depth);
		int maxDepth(TreeNode* node);
		bool isOptimal(TreeNode* node, int depthLimit);

		// builds the tree --> tree is built after the first guess
		TreeNode* buildTree(Vector<std::string>& remaining, int depth);

		// checks exact/partial matches and verifies input
		int intInputCheck(int min, int max);


		// --- testing stuff ---
		void testGetFeedback();
		void testGenerateCodes();
		void testBacktracking();

	public:
		Solver();
		void solve(); // used in Mastermind.cpp (kind of like the UI function for the solver)

		// for testing --> called in Mastermind.cpp
		void test();
};

#endif