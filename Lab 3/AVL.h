#pragma once

#include <iostream>

using namespace std;

class AVL
{
public:
	AVL();
	~AVL();
	void insert(const char word[50]);
	void list();
	void print2D();
	void displayStatistics();
private:
	struct node
	{
		char word[50] = {};
		int count = 1;
		node* leftChild = nullptr;
		node* rightChild = nullptr;
		int balanceFactor = 0;
	};

	void traverseDestruct(node* p); // Traverses through the given node and deletes its children recursively as well as itself
	void list(int& index, node* p); // Traverses through the children of a node as well as the node and prints
	void print2DUtil(node* root, int space);

	node* root = nullptr; // A pointer to the root of the binary search tree. If this is nullptr, no nodes exist in the tree.
};