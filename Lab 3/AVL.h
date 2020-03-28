#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "time.h"

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
	void calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords);
	void calculateNumWords(node* p, unsigned int& numWords, unsigned int& numUniqueWords);
	void list(int& index, node* p); // Traverses through the children of a node as well as the node and prints
	void print2DUtil(node* root, int space);
	unsigned int getHeight();
	void calculateHeight(node* p, unsigned int& height, unsigned int traversalHeight);

	node* root = nullptr; // A pointer to the root of the binary search tree. If this is nullptr, no nodes exist in the tree.

	unsigned long long numberOfBalanceFactorChanges = 0;
	unsigned long long numberOfReferenceChanges = 0;
	unsigned long long numberOfKeyComparisonsMade = 0;
	unsigned long long numberOfNoRotationsNeeded = 0;
	unsigned long long numberOfLeftLeftRotations = 0;
	unsigned long long numberOfLeftRightRotations = 0;
	unsigned long long numberOfRightRightRotations = 0;
	unsigned long long numberOfRightLeftRotations = 0;

	clock_t startTime;
};