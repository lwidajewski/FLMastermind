#ifndef TREE_H
#define TREE_H

#include <string>

#include "Vector.h"

using namespace std;

struct GuessFeedback {
	int exact;
	int partial;

	GuessFeedback();
	GuessFeedback(int e, int p);
	bool isEqual(const GuessFeedback& other) const;
};

struct TreeNode {
	std::string guess;
	GuessFeedback feedback;
	Vector<std::string> remaining;
	Vector<TreeNode*> children;
	TreeNode();
	~TreeNode();
};

class Tree
{
	private:
		TreeNode* root;
	public:
		Tree();

		bool isEmpty();
		TreeNode* getRoot();
		void setRoot(TreeNode* node);

		~Tree();
};

#endif
