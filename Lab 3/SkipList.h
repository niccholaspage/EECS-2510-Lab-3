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

		const static char NEG_INF[50];
		const static char POS_INF[50];
	};

	node* createNegativeInfinityNode();
	node* createPositiveInfinityNode();

	node* find(const char word[50]);

	node* head;
	node* tail;

	int height;
	int amountOfItems;
};