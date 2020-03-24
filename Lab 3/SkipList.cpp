#include "SkipList.h"

const char SkipList::node::NEG_INF[50] = { '.' };
const char SkipList::node::POS_INF[50] = { ',' };

SkipList::SkipList()
{
	head = createNegativeInfinityNode();
	tail = createPositiveInfinityNode();

	head->right = tail;
	tail->left = head;

	amountOfItems = 0;
	height = 1;

	srand(time(NULL));
}

SkipList::node* SkipList::createNegativeInfinityNode()
{
	node* newNode = new node();

	strcpy(newNode->word, node::NEG_INF);

	return newNode;
}

SkipList::node* SkipList::createPositiveInfinityNode()
{
	node* newNode = new node();

	strcpy(newNode->word, node::POS_INF);

	return newNode;
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
			node* negativeNode = createNegativeInfinityNode();
			node* positiveNode = createPositiveInfinityNode();

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

	while (strcmp(p->word, node::POS_INF) != 0)
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