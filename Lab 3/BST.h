//==============================================================================================
// File: BST.h - Binary search tree
// Header for BST.cpp
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

class BST
{
public:
	BST();
	~BST();

	void insert(const char word[50]);
	void list(); // Prints a list of all words in the tree.
	void print2D();
	void displayStatistics();
private:
	struct node
	{
		char word[50] = {};
		int count = 1;
		node* leftChild = nullptr;
		node* rightChild = nullptr;
	};

	void traverseDestruct(node* p); // Traverses through the given node and deletes its children recursively as well as itself
	void calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords);
	void calculateNumWords(node* p, unsigned int& numWords, unsigned int& numUniqueWords);
	void list(int& index, node* p); // Traverses through the children of a node as well as the node and prints
	void print2DUtil(node* root, int space);
	unsigned int getHeight();
	void calculateHeight(node* p, unsigned int& height, unsigned int traversalHeight);

	node* root = nullptr; // A pointer to the root of the binary search tree. If this is nullptr, no nodes exist in the tree.

	unsigned long long numberOfReferenceChanges = 0;
	unsigned long long numberOfKeyComparisonsMade = 0;

	clock_t startTime;
};