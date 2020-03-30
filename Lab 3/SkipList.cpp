//==============================================================================================
// File: SkipList.cpp - Skip list implementation
// c.f.: SkipList.h
//
// This class implements a Skip list, filled with nodes as declared in SkipList.h.  
// Each node contains a word, the number of times the word has appeared, pointers to 
// the left, right, up and down nodes, and a boolean representing whether a node is sentinel.
// This datatype does not perform any rotations like the trees in this project, but instead is
// a lane system where nodes are inserted into fast lanes above the slow lane based on a coin flip.
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Mar 31, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

#include "SkipList.h"

SkipList::SkipList()
{
	// constructor -- we set our start time and initialize the head and
	// tail nodes of our skip list. We also intialize the number of items
	// to zero and the height of the list to one because we only have one
	// lane so far.
	//
	startTime = clock();  // We set the starting time position to the current time.

	head = createSentinelNode(); // We set our head and tail pointers to new sentinel nodes
	tail = createSentinelNode();

	head->right = tail; // Our tail node is to the right of our head node, so we set head's right
	tail->left = head;	// aswell as tail's left.

	numberOfItems = 0;	// We start with zero items in the list.
	height = 1;			// We also start with a height of 1, as we only have one lane in the skip list in the beginning.

	// We construct a Mersenne Twister Engine to handle our
	// random number generation, seeding it with the current time.
	coin = mt19937(time(NULL));
}

SkipList::~SkipList()
{
	// On deconstruction, we have to delete the entire list by
	// deleting each node in the list, one by one.
	//
	node* p = head; // We start at the head of the list.

	while (p != nullptr) // While p is not nullptr,
	{
		node* q = p->right;		// We set q to refer to p's right so we can delete the entire row.

		while (q != nullptr)	// While q is not nullptr,
		{
			node* del = q;	// We set a deletion pointer so we can go to the right of q before we delete it,

			q = q->right;	// we set q to q's right,

			delete del;		// and we delete our deletion pointer.
		}

		node* del = p;	// We set a deletion pointer so we can go below p before we delete it,

		p = p->down;	// set p to be p's down pointer,

		delete del;		// and delete our deletion pointer.
	}
}

SkipList::node* SkipList::createSentinelNode()
{
	// This method is a convenience method to create a sentinel node,
	// so that we don't have to repeatedly set isSentinel on construction
	// of a sentinel node.
	//
	node* newNode = new node(); // We construct a new node,

	newNode->isSentinel = true;	// mark it as sentinel,

	return newNode;				// and return the node pointer.
}

SkipList::node* SkipList::search(const char word[50], bool& found)
{
	// This method searches through the skip list for a node in the slow lane with
	// the given word. If a node with the word is found, it is returned and the
	// boolean found will be set to true. If it is not found, the method will return
	// the node just before where the one we are looking for would be if it was in the
	// list.
	// This searching code is from an e-mail he sent, check it when doing commenting - email sent on Tuesday, March 24
	node* p = head; // Start at the head since we will be searching from the beginning of the list

	while (true) // We want to loop until we find a node.
	{
		while (!p->right->isSentinel) // While the node to the right of p is not a sentinel node,
		{
			int compareValue = strcmp(p->right->word, word); // compare the right pointer's word to the word we are looking for.

			numberOfKeyComparisonsMade++; // Increment our key comparisons since we just made one

			if (compareValue < 0)	// If the right pointer's word is less than our word,
			{
				p = p->right;		// we advance on to the next pointer.
			}
			else if (compareValue == 0) // If the right pointer's word is equal to our word, we found it!
			{
				p = p->right; // We advance to p's right node.

				// Since this method only returns nodes in the slow lane, we have to go to the slow lane.
				while (p->down != nullptr)	// While a node below p exists,
				{
					p = p->down;			// p becomes the node under p.
				}

				found = true; // Since we've successfully found a pointer with the exact word, we set found to true.

				return p; // Now that we have the node, we simply return it.
			}
			else		// At this point, the right pointer's word is greater than our word,
			{
				break;	// so we break.
			}
		}

		if (p->down == nullptr) // We can't go down any further, so there are no other lanes to check.
		{
			found = false;	// We know there is no node with the word in it at this point, so we set found to false.

			return p;		// We now return p, the node just before the one we are looking for if it was in the list.
		}
		else				// Otherwise, we can go down a node,
		{
			p = p->down;	// so we set p to the node below p.
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

	numberOfReferenceChanges += 2;

	p->right->left = newNode;	// We get the node's right link and set its left child to our new node.
	p->right = newNode;			// We then set p's right child to be the new node.

	numberOfReferenceChanges += 2;

	numberOfItems++;

	int currentHeight = 1;

	while (coin() & 1)
	{
		currentHeight++;
		numberOfHeadsCoinTosses++;

		node* pileNode = new node();

		pileNode->down = newNode;
		newNode->up = pileNode;

		numberOfReferenceChanges += 2;

		strcpy(pileNode->word, word);

		if (currentHeight > height)
		{
			node* negativeNode = createSentinelNode();
			node* positiveNode = createSentinelNode();

			negativeNode->down = head;
			head->up = negativeNode;
			numberOfReferenceChanges += 2;

			positiveNode->down = tail;
			tail->up = positiveNode;
			numberOfReferenceChanges += 2;

			negativeNode->right = pileNode;
			pileNode->left = negativeNode;
			positiveNode->left = pileNode;
			pileNode->right = positiveNode;
			numberOfReferenceChanges += 4;

			head = negativeNode;
			tail = positiveNode;
			numberOfReferenceChanges += 2;

			height++;
		}
		else
		{
			node* leftNode = newNode->left;

			while (leftNode->up == nullptr)
			{
				leftNode = leftNode->left;
			}

			node* rightNode = newNode->right;

			while (rightNode->up == nullptr)
			{
				rightNode = rightNode->right;
			}

			pileNode->left = leftNode->up;
			pileNode->right = rightNode->up;
			leftNode->up->right = pileNode;
			rightNode->up->left = pileNode;

			numberOfReferenceChanges += 4;
		}

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

		if (index != numberOfItems)
		{
			cout << ", ";
		}

		start = start->right;
	}

	cout << endl;
}

void SkipList::larryList()
{
	// This method displays the nodes in the skip list sideways so that we can
	// easily view each lane and node in the skip list.
	//
	node* p = head; // Set our pointer p to the head. We will use this to get to the slowest lane's head.

	while (p->down != nullptr)	// While the pointer below p isn't null,
	{
		p = p->down;			// we set p to it.
	}

	p = p->right; // We don't want to print the head, so we set to p to p's right node.

	while (!p->isSentinel) // While p isn't a sentinel node,
	{
		node* q = p; // We will set a new pointer q to p.

		do
		{
			cout << q->word << " ";	// Print node q's word,

			q = q->up;				// and set q to q's upper node
		} while (q != nullptr);		// while q isn't null.

		cout << endl; // We've finished printing this column of nodes, we print out a newline.

		p = p->right; // Set p to p's right because we are now going to repeat this with the next column of nodes.
	}
}

void SkipList::calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords)
{
	numWords = 0;
	numUniqueWords = 0;

	node* p = head;

	while (p->down != nullptr)
	{
		p = p->down;
	}

	while (!p->right->isSentinel)
	{
		numWords += p->count;
		numUniqueWords += 1;

		p = p->right;
	}
}

unsigned int SkipList::getTotalNodes()
{
	unsigned int totalNodes = 0;

	node* p = head;

	while (p != nullptr)
	{
		node* q = p;

		while (!q->right->isSentinel)
		{
			totalNodes++;

			q = q->right;
		}

		p = p->down;
	}

	return totalNodes;
}

void SkipList::displayStatistics()
{
	// This method displays statistics related to the number of words in the list and
	// insertion statistics as nodes are inserted into the skip list. It also
	// displays the elapsed time since the construction of the skip list.
	//
	// We get the current time, subtract the start time from it, and divide by 1000
	// to get the elapsed time in seconds. We calculate the elapsed time here since
	// we don't want our printing and calculating of stats to be included in the elapsed
	// time.
	double elapsedTime = (clock() - startTime) / 1000.0;

	cout << "Skip List Stats:\n"; // Print out a header specifying that these are skip list stats
	cout << "Reference Changes: " << numberOfReferenceChanges << "\n"; // Print out the number of reference changes made
	cout << "Key Comparisons: " << numberOfKeyComparisonsMade << "\n"; // Print out the number of key comparisons made
	cout << "Coin Tosses that were heads: " << numberOfHeadsCoinTosses << "\n"; // Print out the number of coin tosses that resulted in heads
	cout << "Number of Items: " << numberOfItems << "\n"; // Print out the number of items in the skip list
	cout << "Total Nodes: " << getTotalNodes() << "\n"; // Print out the total number of nodes in the list (meaning every node in every lane)

	unsigned int numWords, numUniqueWords; // Declare variables for the total number of words and unique words in the list

	// Call the calculateNumWords, passing in our word counts by reference so that they are set to the proper values
	calculateNumWords(numWords, numUniqueWords);

	cout << "Words: " << numWords << "\n"; // Print out the total number of words
	cout << "Unique Words / Slow Lane Nodes: " << numUniqueWords << "\n"; // Print out the total number of unique words / slow lane nodes
	cout << "Height: " << height << "\n"; // Print out the height of the skip list
	cout << "Elapsed Time: " << elapsedTime << " seconds\n"; // Print out the elapsed time we calculated at the beginning of the method.
}