//==============================================================================================
// File: RBT.h - Red-Black tree
// Header for RBT.cpp
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Mar 31, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "time.h"

using namespace std;

class RBT
{
public:
	RBT();
	~RBT();

	void insert(const char word[50]); // adds a word to the tree
	void list(); // Prints a list of all words in the tree.
	void displayStatistics(); // Displays statistics about the tree operations performed.
private:
	struct node
	{
		char word[50] = {};			// The word of the node
		unsigned int count = 1;		// The amount of times the word has been inserted into the binary search tree
		bool color = RED;			// The color of the node. Since we only have two possible colors, red or black, it is just a bool.
		node* parent = nullptr;		// A pointer to the parent of the node
		node* leftChild = nullptr;	// A pointer to the left child of the node
		node* rightChild = nullptr;	// A pointer to the right child of the node
	};

	// As seen from our node struct above, the node color is a boolean, so we just make RED false and BLACK true.
	// We will always refer to these colors with these constant static RED and BLACK variables instead of true or false.
	const static bool RED = false;
	const static bool BLACK = true;

	void traverseDestruct(node* p); // Traverses through the given node and deletes its children recursively as well as itself

	// Calculates the number of words and unique words in the RBT tree.
	void calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords);

	// Recursively calculates the number of words and unique words by counting the given node's words and its two children.
	void calculateNumWords(node* p, unsigned int& numWords, unsigned int& numUniqueWords);

	void list(int& index, node* p);	// Traverses through the children of a node as well as the node and prints
	void leftRotate(node* p);		// Performs a left rotation on the given node
	void rightRotate(node* p);		// Performs a right rotation on the given node
	void insertFixup(node* p);		// Fixes up the insertion into the tree of the given node by performing rotations and recoloring nodes as needed.
	unsigned int getHeight();		// Returns the height of the tree

	// Recursively calculates the height of the tree by traversing through the given node and its children
	void calculateHeight(node* p, unsigned int& height, unsigned int traversalHeight);

	// A pointer to the root of the binary search tree. While this is first set to nullptr,
	// it will point to the nil node pointer below after the constructor has been called.
	// If this points to the same node that nil points to, no nodes exist in the tree.
	node* root = nullptr;

	// A pointer to a nil node that we will set up in the constructor. nil is a special node,
	// who's parent, left child, and right child are all nil. It is used instead of nullptr
	// for "null" root, left child, and right child nodes to make rotations and recoloring
	// easier to implement.
	node* nil = nullptr;

	unsigned long long numberOfRecolorings = 0;			// Keeps track of the number of times a node gets recolored.
	unsigned long long numberOfReferenceChanges = 0;	// Keeps track of the number of times the root pointer or child pointers change.
	unsigned long long numberOfKeyComparisonsMade = 0;	// Keeps track of the number of word key comparisons during tree inserts.
	unsigned long long numberOfNoFixesNeeded = 0;		// Keeps track of the number of times a new node is inserted without a fix performed on the tree.
	unsigned long long numberOfCase1Fixes = 0;			// Keeps track of the number of times a case 1 fix is performed on new node insertion.
	unsigned long long numberOfCase2Fixes = 0;			// Keeps track of the number of times a case 2 fix is performed on new node insertion.
	unsigned long long numberOfCase3Fixes = 0;			// Keeps track of the number of times a case 3 fix is performed on new node insertion.

	clock_t startTime; // A variable that represents the time at very beginning of tree construction
};