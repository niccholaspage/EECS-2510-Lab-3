#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

class SkipList
{
public:
	SkipList();
	~SkipList();

	void insert(const char word[50]);

	void list();

	void larryList();
private:
	struct node
	{
		char word[50] = {};
		int count = 1;

		node* up = nullptr;
		node* down = nullptr;
		node* left = nullptr;
		node* right = nullptr;

		bool isSentinel = false;
	};

	node* createNegativeInfinityNode();
	node* createPositiveInfinityNode();

	node* search(const char word[50], bool& found);

	node* head;
	node* tail;

	int height;
	int amountOfItems;
	int numberOfComparisonsMade;
};