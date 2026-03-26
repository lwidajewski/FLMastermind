#ifndef SEQUENCE_H
#define	SEQUENCE_H

struct Node {
	char color;
	Node* next; //for sequence
	Node* left; //for tree
	Node* right; //for tree
};

class Sequence
{
public:
	Sequence();

	bool isEmpty();
	void print();
	Node* getHead();
	int length();

	void insertAtEnd(char c);
	void insertAtStart(char c);
	void clear();

	~Sequence();

private:
	Node* head;
};

#endif