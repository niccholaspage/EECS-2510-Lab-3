//==============================================================================================
// File: AVL.h - Adelson-Velsky and Landis tree
// Header for AVL.cpp
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Apr 4, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

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

	void insert(const char word[50]); // adds a word to the tree
	void list(); // Prints a list of all words in the tree.
	void displayStatistics(); // Displays statistics about the tree operations performed.
private:
	struct node
	{
		char word[50] = {};			// The word of the node
		unsigned int count = 1;		// The amount of times the word has been inserted into the binary search tree
		node* leftChild = nullptr;	// A pointer to the left child of the node
		node* rightChild = nullptr;	// A pointer to the right child of the node
		int balanceFactor = 0;		// The balance factor of the node, based on its children
	};

	void traverseDestruct(node* p); // Traverses through the given node and deletes its children recursively as well as itself

	// Calculates the number of words and unique words in the AVL tree.
	void calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords);

	// Recursively calculates the number of words and unique words by counting the given node's words and its two children.
	void calculateNumWords(node* p, unsigned int& numWords, unsigned int& numUniqueWords);

	void list(int& index, node* p); // Traverses through the children of a node as well as the node and prints

	unsigned int getHeight(); // Returns the height of the tree

	// Recursively calculates the height of the tree by traversing through the given node and its children
	void calculateHeight(node* p, unsigned int& height, unsigned int traversalHeight);

	node* root = nullptr; // A pointer to the root of the binary search tree. If this is nullptr, no nodes exist in the tree.

	unsigned long long numberOfBalanceFactorChanges = 0;		// Keeps track of the number of times a node's balance factor was changed
	unsigned long long numberOfReferenceChanges = 0;			// Keeps track of the number of times the root pointer or child pointers change.
	unsigned long long numberOfKeyComparisonsMade = 0;			// Keeps track of the number of word key comparisons during tree inserts.
	unsigned long long numberOfNoRotationsNeeded = 0;			// Keeps track of the number of times a new node is inserted and no rotation was performed.
	unsigned long long numberOfLeftLeftRotations = 0;			// Keeps track of the number of times a left left rotation is performed on node insert
	unsigned long long numberOfLeftRightRotations = 0;			// Keeps track of the number of times a left right rotation is performed on node insert
	unsigned long long numberOfRightRightRotations = 0;			// Keeps track of the number of times a right right rotation is performed on node insert
	unsigned long long numberOfRightLeftRotations = 0;			// Keeps track of the number of times a right left rotation is performed on node insert
	unsigned long long numberOfAToYPasses = 0;					// Keeps track of the number of times we go from A to Y
	unsigned long long numberOfAToYBalanceFactorChanges = 0;	// Keeps track of the number of times we change balance factors from A to Y
};