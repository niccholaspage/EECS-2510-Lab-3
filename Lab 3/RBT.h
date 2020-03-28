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
		char word[50] = {};
		int count = 1;
		bool color = RED; // true is black, false is red
		node* parent = nullptr;
		node* leftChild = nullptr;
		node* rightChild = nullptr;
	};

	const static bool BLACK = true;
	const static bool RED = false;

	void traverseDestruct(node* p); // Traverses through the given node and deletes its children recursively as well as itself
	void calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords);
	void calculateNumWords(node* p, unsigned int& numWords, unsigned int& numUniqueWords);
	void list(int& index, node* p); // Traverses through the children of a node as well as the node and prints
	void leftRotate(node* p);
	void rightRotate(node* p);
	void insertFixup(node* p);
	unsigned int getHeight();
	void calculateHeight(node* p, unsigned int& height, unsigned int traversalHeight);

	node* root = nullptr; // A pointer to the root of the binary search tree. If this is nullptr, no nodes exist in the tree.
	node* nil = nullptr;

	unsigned long long numberOfRecolorings = 0;
	unsigned long long numberOfReferenceChanges = 0;
	unsigned long long numberOfKeyComparisonsMade = 0;
	unsigned long long numberOfNoFixesNeeded = 0;
	unsigned long long numberOfCase1Fixes = 0;
	unsigned long long numberOfCase2Fixes = 0;
	unsigned long long numberOfCase3Fixes = 0;

	clock_t startTime;
};