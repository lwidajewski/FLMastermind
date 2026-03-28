#include <iostream>
#include <string>

#include "Tree.h"


// ---------- GuessFeedback Struct ----------
GuessFeedback::GuessFeedback() {
	exact = 0;
	partial = 0;
};

GuessFeedback::GuessFeedback(int e, int p) {
	exact = e;
	partial = p;
};

bool GuessFeedback::isEqual(const GuessFeedback& other) const {
	if (exact == other.exact && partial == other.partial) {
		return true;
	}
	else {
		return false;
	};
};

// ---------- TreeNode Struct ----------
TreeNode::TreeNode() : remaining(1296), children(14) {};

TreeNode::~TreeNode() {
	for (int i = 0; i < children.size(); i++) {
		delete children.at(i);
	};
};

// ---------- Tree Functions ----------
Tree::Tree() {
	root = nullptr;
};

bool Tree::isEmpty() {
	if (root == nullptr) {
		return true;
	}
	else {
		return false;
	};
};

TreeNode* Tree::getRoot() {
	return root;
};

void Tree::setRoot(TreeNode* node) {
	if (!isEmpty()) {
		delete root;
	};
	root = node;
};

Tree::~Tree() {
	delete root;
};