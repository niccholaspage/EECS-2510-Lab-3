//==============================================================================================
// File: SkipList.h - Skip list
// Header for SkipList.cpp
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Apr 4, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <random>
#include <iostream>

using namespace std;

class SkipList
{
public:
	SkipList();
	~SkipList();

	void insert(const char word[50]); // adds a word to the skip list
	void list(); // Prints a list of all words in the skip list.
	void displayStatistics(); // Displays statistics about the skip list operations performed.
	void larryList(); // Displays the skip list sideways so that all of the nodes and lanes can be seen
private:
	struct node
	{
		char word[50] = {};
		unsigned int count = 1;

		node* up = nullptr;
		node* down = nullptr;
		node* left = nullptr;
		node* right = nullptr;

		bool isSentinel = false;
	};

	node* createSentinelNode();

	node* search(const char word[50], bool& found);

	void calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords);
	unsigned int getTotalNodes();

	mt19937 coin;

	node* head;
	node* tail;

	int height;
	int numberOfItems;
	unsigned long long numberOfKeyComparisonsMade = 0;
	unsigned long long numberOfReferenceChanges = 0;
	unsigned long long numberOfHeadsCoinTosses = 0;
};