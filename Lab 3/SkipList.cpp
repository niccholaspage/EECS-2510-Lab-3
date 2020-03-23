#include "SkipList.h"

SkipList::SkipList()
{
	head = new node();
	tail = new node();

	strcpy(head->word, node::NEG_INF);
	strcpy(tail->word, node::POS_INF);

	head->right = tail;
	tail->left = head;

	amountOfItems = 0;
	height = 1;

	srand(time());
}

SkipList::node* SkipList::find(const char word[50])
{
	node* p = head;

	while (true)
	{
		while (strcmp(p->right->word, node::POS_INF) != 0 && strcmp(p->right->word, word) <= 0)
		{
			p = p->right;
		}

		if (p->down == nullptr)
		{
			return p;
		}

		p = p->down;
	}
}

void SkipList::insert(const char word[50])
{
	node* p = find(word);

	if (strcmp(p->word, word) == 0)
	{
		p->count++;
	}
	else
	{
		node* newNode = new node();

		// Specify defaults if you want because Dr. Thomas for some reason likes to be super redundant and uncool

		strcpy(newNode->word, word);

		p->right->left = newNode;	// We get the node's right link and set its left child to our new node.
		p->right = newNode;			// We then set p's right child to be the new node.

		int currentHeight = 1;

		while (rand() & 1)
		{
			currentHeight++;

			if (currentHeight > height)
			{

			}
		}
	}
}