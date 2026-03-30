#include <iostream>
#include <string>

#include "Tree.h"


// ---------- GuessFeedback Struct ----------
// constructor
GuessFeedback::GuessFeedback() {
	exact = 0;
	partial = 0;
};

// constructor for when matches are passed in
GuessFeedback::GuessFeedback(int e, int p) {
	exact = e;
	partial = p;
};

// checks if GuessFeedback has the same exact/partial matches as another GuessFeedback
bool GuessFeedback::isEqual(const GuessFeedback& other) const {
	if (exact == other.exact && partial == other.partial) {
		return true;
	}
	else {
		return false;
	};
};

// ---------- TreeNode Struct ----------
// constructor
TreeNode::TreeNode() : remaining(1296), children(14) {};

// destructor
TreeNode::~TreeNode() {
	for (int i = 0; i < children.size(); i++) {
		delete children.at(i);
	};
};

// ---------- Tree Functions ----------
// constructor
Tree::Tree() {
	root = nullptr;
};

// checks if tree is empty or not
bool Tree::isEmpty() {
	if (root == nullptr) {
		return true;
	}
	else {
		return false;
	};
};

// get the root node
TreeNode* Tree::getRoot() {
	return root;
};

// set the root node
void Tree::setRoot(TreeNode* node) {
	if (!isEmpty()) {
		delete root;
	};
	root = node;
};

// destructor
Tree::~Tree() {
	delete root;
};