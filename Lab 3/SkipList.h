#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string.h>

using namespace std;

class SkipList
{
public:
	SkipList();

	void insert(const char word[50]);

	void traverse();
private:
	struct node
	{
		char word[50] = {};
		int count = 1;

		node* up = nullptr;
		node* down = nullptr;
		node* left = nullptr;
		node* right = nullptr;

		const static char NEG_INF = '.';
		const static char POS_INF = ',';
	};

	node find(const char word[50]);

	node* head;
	node* tail;

	int height;
	int amountOfItems;
};