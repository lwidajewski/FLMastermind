#include "Sequence.h"
#include <iostream>

using namespace std;

Sequence::Sequence() {
	head = nullptr;
};

bool Sequence::isEmpty() {
	if (head == nullptr)
		return true;
	else
		return false;
};

void Sequence::print() {
	Node* current = head;

	if (!isEmpty()) {
		while (current->next != nullptr) {
			cout << current->color << ", ";
			current = current->next;
		}
		cout << current->color << endl;
	}
	else {
		cout << "Sequence empty" << endl;
	};
};

Node* Sequence::getHead() {
	return head;
};

int Sequence::length() {
	int count = 0;
	Node* current = head;
	while (current != nullptr) {
		count++;
		current = current->next;
	};
	return count;
};

void Sequence::insertAtEnd(char c) {
	if (isEmpty())
		insertAtStart(c);
	else {
		//makes new node with inputed color
		Node* temp = new Node;
		temp->color = c;

		//finds the last node in the sequence
		Node* current = head;
		while (current->next != nullptr) {
			current = current->next;
		};

		//once found, points the current last node to the temp node
		current->next = temp;
		temp->next = nullptr;
	};
};

void Sequence::insertAtStart(char c) {
	//makes new node and assigns color
	Node* temp = new Node;
	temp->color = c;

	//points whatever used to be head, if anything, to the next node
	if (isEmpty()) {
		temp->next = nullptr;
	}
	else {
		temp->next = head;
	};

	//assigns temp as the new head
	head = temp;
};

void Sequence::clear() {
	Node* current = head;
	while (current != nullptr) {
		Node* next = current->next;
		delete current;
		current = next;
	};
	head = nullptr;
};

Sequence::~Sequence() {
	Node* current = head;
	while (current != nullptr) {
		Node* next = current->next;
		delete current;
		current = next;
	};
	head = nullptr;
};