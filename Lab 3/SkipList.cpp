#include "SkipList.h"

SkipList::SkipList()
{
	head = createSentinelNode();
	tail = createSentinelNode();

	head->right = tail;
	tail->left = head;

	amountOfItems = 0;
	height = 1;
	numberOfComparisonsMade = 0;

	srand(time(NULL));
}

SkipList::~SkipList()
{
	node* p = head;

	while (p != nullptr)
	{
		node* q = p->right;

		while (q != nullptr)
		{
			node* del = q;

			q = q->right;

			delete del;
		}

		node* del = p;

		p = p->down;

		delete del;
	}
}

SkipList::node* SkipList::createSentinelNode()
{
	node* newNode = new node();

	newNode->isSentinel = true;

	return newNode;
}

SkipList::node* SkipList::search(const char word[50], bool& found)
{
	// This searching code is from an e-mail he sent, check it when doing commenting - email sent on Tuesday, March 24
	node* p = head;

	while (true)
	{
		while (!p->right->isSentinel)
		{
			int compareValue = strcmp(p->right->word, word);

			numberOfComparisonsMade++;

			if (compareValue < 0)
			{
				p = p->right;
			}
			else if (compareValue == 0)
			{
				p = p->right;

				while (p->down != nullptr)
				{
					p = p->down;
				}

				found = true;

				return p;
			}
			else
			{
				break;
			}
		}

		if (p->down == nullptr)
		{
			found = false;

			return p;
		}
		else
		{
			p = p->down;
		}
	}
}

void SkipList::insert(const char word[50])
{
	bool found = false;

	node* p = search(word, found);

	if (found)
	{
		p->count++;

		return;
	}

	node* newNode = new node();

	// Specify defaults if you want because Dr. Thomas for some reason likes to be super redundant and uncool

	strcpy(newNode->word, word);

	newNode->left = p;
	newNode->right = p->right;

	p->right->left = newNode;	// We get the node's right link and set its left child to our new node.
	p->right = newNode;			// We then set p's right child to be the new node.

	amountOfItems++;

	int currentHeight = 1;

	while (rand() & 1)
	{
		currentHeight++;

		if (currentHeight > height)
		{
			node* negativeNode = createSentinelNode();
			node* positiveNode = createSentinelNode();

			negativeNode->down = head;
			head->up = negativeNode;

			positiveNode->down = tail;
			tail->up = positiveNode;

			negativeNode->right = positiveNode;
			positiveNode->left = negativeNode;

			head = negativeNode;
			tail = positiveNode;

			height++;
		}

		node* pileNode = new node();

		pileNode->down = newNode;
		newNode->up = pileNode;

		strcpy(pileNode->word, word);

		node* leftNode = p;

		while (leftNode->up == nullptr)
		{
			leftNode = leftNode->left;
		}

		node* rightNode = p;

		while (rightNode->up == nullptr)
		{
			rightNode = rightNode->right;
		}

		pileNode->left = leftNode->up;
		pileNode->right = rightNode->up;

		newNode = pileNode;
	}
}

void SkipList::list()
{
	node* start = head;

	while (start->down != nullptr)
	{
		start = start->down;
	}

	start = start->right;

	node* end = tail;

	while (end->down != nullptr)
	{
		end = end->down;
	}

	int index = 0;

	cout << "Set contains: "; // so we print out "Set contains: " as a prefix,

	while (start != end)
	{
		cout << "(" << ++index << ") " << start->word << " " << start->count;

		if (index != amountOfItems)
		{
			cout << ", ";
		}

		start = start->right;
	}
}

void SkipList::larryList()
{
	cout << "Height: " << height << ", amount of items: " << amountOfItems << "\n";

	node* p = head;

	while (p->down != nullptr)
	{
		p = p->down;
	}

	p = p->right;

	while (!p->isSentinel)
	{
		node* q = p;

		do
		{
			cout << q->word << " ";

			q = q->up;
		} while (q != nullptr);

		cout << endl;

		p = p->right;
	}
}

void SkipList::displayStatistics()
{
	// TODO: Implement method
}