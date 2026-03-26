#include <iostream>
#include <string>

#include "Tree.h"

Tree::Tree() {
	root = nullptr;
};

TreeNode* Tree::clone(TreeNode* curr) {
	if (curr == nullptr) {
		return nullptr;
	}
	else {
		TreeNode* temp = new TreeNode;
		temp->data = curr->data;
		clone(curr->left);
		clone(curr->right);
		return temp;
	};
};

bool Tree::isEmpty() {
	if (root == nullptr) {
		return true;
	}
	else {
		return false;
	};
};

void Tree::insert(Sequence s) {
	TreeNode* temp = new TreeNode;
	temp->data = s;
	temp->left = nullptr;
	temp->right = nullptr;

	if (isEmpty()) {
		root = temp;
	}
	else {
		TreeNode* curr = root;
		while (curr != nullptr) {
			if (checkInTree(curr->data, temp->data) == "left") {
				curr = curr->left;
			}
			else if (checkInTree(curr->data, temp->data) == "right") {
				curr = curr->left;
			}
			else {
				break;
			};
		};
		if (checkInTree(curr->data, temp->data) == "left") {
			curr = curr->left;
		}
	};
};

void Tree::printTree() {

};

void Tree::printTree(TreeNode* curr) {

};

string Tree::checkInTree(Sequence curr, Sequence temp) {
	/*checks to see where a sequence is in the tree:
					U
			B				L
		G		Y		W		O
	R								P
	*/

	if (curr == nullptr) {
		return "null";
	}
};