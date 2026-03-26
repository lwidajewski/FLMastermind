#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>

#include "Sequence.h"

using namespace std;

struct TreeNode {
	Sequence data;
	TreeNode* left;
	TreeNode* right;
};

class Tree
{
	public:
		Tree();
		TreeNode* clone(TreeNode* curr);
		bool isEmpty();
		void insert(Sequence s);
		void printTree();
		void printTree(TreeNode* curr);
		std::string checkInTree(Sequence curr, Sequence temp);		//used to check where in a tree the sequence used might be
	private:
		TreeNode* root;
};

#endif
