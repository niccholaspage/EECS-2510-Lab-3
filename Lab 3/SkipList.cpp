#include "SkipList.h"

SkipList::SkipList()
{
	head = new node();
	tail = new node();

	char negativeInfinity[50] = { node::NEG_INF };
	char positiveInfinity[50] = { node::POS_INF };

	strcpy(head->word, negativeInfinity);
	strcpy(tail->word, positiveInfinity);

	head->right = tail;

	amountOfItems = 0;
	height = 1;
}