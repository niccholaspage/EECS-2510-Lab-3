//==============================================================================================
// File: RBT.cpp - Red-Black tree implementation
// c.f.: RBT.h
//
// This class implements an RBT tree, filled with nodes as declared in RBT.h.  
// Each node contains a word, the number of times the word has appeared, pointers to 
// the left and right child of the node, and the color of the node, which is either red or black.
// This tree performs different types of rotations based on the color of nodes in the tree as nodes
// are inserted into it.
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Mar 31, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

#include "RBT.h"

RBT::RBT()
{
	// constructor -- there's nothing to do other then setting our start
	// time, initializing our nil node, and making sure the root pointer is nil.
	//
	startTime = clock(); // We set the starting time position to the current time.

	nil = new node();

	strcpy(nil->word, "");
	nil->color = BLACK;
	nil->parent = nil->leftChild = nil->rightChild = nil;

	root = nil;
}

RBT::~RBT()
{
	if (root != nil)
	{
		traverseDestruct(root);
	}

	delete nil;

	nil = nullptr;
	root = nullptr;
}

void RBT::traverseDestruct(node* p) {
	// This recursive method can be called on a given node to delete
	// its children and itself. Doing this recursively ensures that
	// every node will get deleted as long as this method is initially
	// called on the root node.
	//
	if (p->leftChild != nil) // Check if the left child of the node is not null
	{
		traverseDestruct(p->leftChild); // Since the node does have a left child, we will call the traverseDestruct method on the node
	}

	if (p->rightChild != nil) // Check if the right child is not null
	{
		traverseDestruct(p->rightChild); // Since the node does have a right child, we will call the traverseDestruct method on the node
	}

	// Now that we have properly deleted the children of the given node,
	// we are safe to delete the node since we have already taken care of
	// its children.
	//
	delete p;
}

void RBT::leftRotate(node* x)
{
	// The pseudocode for this can be found in Lecture 11, slide 18. Comments are provided.
	node* y = x->rightChild;

	x->rightChild = y->leftChild;
	numberOfReferenceChanges++;

	if (y->leftChild != nil)
	{
		y->leftChild->parent = x;
		numberOfReferenceChanges++;
	}

	y->parent = x->parent;
	numberOfReferenceChanges++;

	if (x->parent == nil)
	{
		root = y;
	}
	else if (x == x->parent->leftChild)
	{
		x->parent->leftChild = y;
	}
	else
	{
		x->parent->rightChild = y;
	}

	numberOfReferenceChanges++;

	y->leftChild = x;

	x->parent = y;

	numberOfReferenceChanges += 2;
}

void RBT::rightRotate(node* x)
{
	node* y = x->leftChild;

	x->leftChild = y->rightChild;
	numberOfReferenceChanges++;

	if (y->rightChild != nil)
	{
		y->rightChild->parent = x;
		numberOfReferenceChanges++;
	}

	y->parent = x->parent;
	numberOfReferenceChanges++;

	if (x->parent == nil)
	{
		root = y;
	}
	else if (x == x->parent->rightChild)
	{
		x->parent->rightChild = y;
	}
	else
	{
		x->parent->leftChild = y;
	}

	numberOfReferenceChanges++;

	y->rightChild = x;

	x->parent = y;

	numberOfReferenceChanges += 2;
}

void RBT::insert(const char word[50])
{
	node* y = nil;
	node* x = root;

	while (x != nil)
	{
		y = x;

		int compareValue = strcmp(word, x->word);

		numberOfKeyComparisonsMade++;

		if (compareValue < 0)
		{
			x = x->leftChild;
		}
		else if (compareValue > 0)
		{
			x = x->rightChild;
		}
		else
		{
			x->count++;

			return;
		}
	}

	node* z = new node();
	strcpy(z->word, word);

	z->parent = y;
	numberOfReferenceChanges++;

	if (y == nil)
	{
		root = z;
		numberOfReferenceChanges++;
	}
	else
	{
		if (strcmp(z->word, y->word) < 0)
		{
			y->leftChild = z;
		}
		else
		{
			y->rightChild = z;
		}

		numberOfReferenceChanges++;
		numberOfKeyComparisonsMade++;
	}

	z->leftChild = z->rightChild = nil;
	z->color = RED;

	insertFixup(z);
}

void RBT::insertFixup(node* z)
{
	if (z->parent->color == BLACK)
	{
		numberOfNoFixesNeeded++;
	}

	while (z->parent->color == RED)
	{
		if (z->parent == z->parent->parent->leftChild)
		{
			node* y = z->parent->parent->rightChild;

			if (y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				numberOfRecolorings += 3;
				z = z->parent->parent;
				numberOfCase1Fixes++;
			}
			else
			{
				if (z == z->parent->rightChild)
				{
					z = z->parent;

					leftRotate(z);

					numberOfCase2Fixes++;
				}

				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				numberOfRecolorings += 2;

				rightRotate(z->parent->parent);

				numberOfCase3Fixes++;
			}
		}
		else
		{
			node* y = z->parent->parent->leftChild;

			if (y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				numberOfRecolorings += 3;
				z = z->parent->parent;
				numberOfCase1Fixes++;
			}
			else
			{
				if (z == z->parent->leftChild)
				{
					z = z->parent;

					rightRotate(z);

					numberOfCase2Fixes++;
				}

				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				numberOfRecolorings += 2;

				leftRotate(z->parent->parent);

				numberOfCase3Fixes++;
			}
		}
	}

	root->color = BLACK;
	numberOfRecolorings++;
}

void RBT::list()
{
	// This method traverses through the entire tree and prints out the nodes in a nicely formatted list
	// with indexes and the node's word and count.
	//
	if (root == nil) // If the root is nullptr, than there is nothing in the tree,
	{
		cout << "Set is empty" << endl; // so we just print out "Set is empty"
	}
	else // otherwise, we actually have nodes in the tree,
	{
		cout << "Set contains: "; // so we print out "Set contains: " as a prefix,

		int index = 0; // We set up a index variable to 0 that is incremented before printing a node

		list(index, root); // We now start our recursive traversal through the root node with our overloaded list method.

		cout << endl; // We finish off the output by printing out a newline, as we never did it inside the above method call.
	}
};

void RBT::list(int& index, node* p)
{
	// This method does a recursive traversal through all of the nodes in the tree
	// and prints out their word and count with an index parameter. The index
	// parameter is passed by reference so that no matter which recursive call
	// of this method we are in, the correct index will always be printed out.
	// Through talking with Dr. Thomas, I learned that the index variable could
	// have been a class variable instead. I enjoy this method because it does
	// not require any cleanup or setup of a class variable.
	//
	if (p->leftChild != nil)   // If the left child of the node is not null,
	{
		list(index, p->leftChild); // we print out the left child node as well, recursively taking care of its children.

		// Since we have now printed out the left child node, we need to print
		// out a comma to separate it from the node we are about to print.
		//
		cout << ", ";
	}

	// We now print out the index AFTER incrementing it, as well as the node's
	// word and count.
	cout << "(" << ++index << ") " << p->word << " " << p->count;

	if (p->rightChild != nil) // If the right child of the node is not null,
	{
		// we print out a comma to separate the node we already printed from
		// the right child we are about to print.
		cout << ", ";

		list(index, p->rightChild); // we now print out the right child node as well, recursively taking care of its children.
	}
}

void RBT::calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords)
{
	// This method calculates the number of words and unique words in the tree.
	// The method takes two unsigned integer references that it initializes to zero.
	// It then calls the recursive form of this function on the root node. This will
	// go through each node in the tree and increment both word counters in each call.
	//
	numWords = 0;
	numUniqueWords = 0;

	calculateNumWords(root, numWords, numUniqueWords); // We call the calculation method at the root node so we can count the whole tree.
}

void RBT::calculateNumWords(node* p, unsigned int& numWords, unsigned int& numUniqueWords)
{
	// This method does a recursive traversal through all of the nodes in the tree
	// and increments the references to the number of words and unique words through
	// each call.
	//
	if (p->leftChild != nil) // If the left child of the node is not null,
	{
		calculateNumWords(p->leftChild, numWords, numUniqueWords); // we call the method on the left child of the node.
	}

	if (p->rightChild != nil) // If the right child of the node is not null,
	{
		calculateNumWords(p->rightChild, numWords, numUniqueWords); // we call the method on the left child of the node.
	}

	numWords += p->count;	// We increment the number of words by the node's count,
	numUniqueWords += 1;	// and the number of unique words by one, as each node represents a unique word.
}

unsigned int RBT::getHeight()
{
	// This method returns the height of the tree. In this program, the height is NOT
	// the number of child pointer links required to get from the root to the deepest leaf. Instead,
	// it is the amount of levels of a tree. For example, a tree with no nodes has a height of zero,
	// and a tree with only a root node has a height of 1.
	//
	if (root == nil) // If there is no root,
	{
		return 0; // we have no nodes in the tree, so the height is 0.
	}

	unsigned int height = 1; // Since we have at least a root node, we start at a height of one.

	// We call our calculate height method on the root node, which will recursively go through
	// each node in the tree, and properly update our height variable with the height of the entire tree.
	calculateHeight(root, height, height);

	return height;
}

void RBT::calculateHeight(node* p, unsigned int& height, unsigned int traversalHeight)
{
	// This method does a recursive traversal of the given node and gets called on every node
	// underneath it. As it traverses through each node, the traversal height gets incremented
	// by one, and the height gets changed to the traversal height if the traversal height is greater
	// than it.
	//
	if (p->leftChild != nil) // If p has a left child,
	{
		// we recursively call calculateHeight on it, incrementing the traversal height by one since we have gone down a level of the tree.
		calculateHeight(p->leftChild, height, traversalHeight + 1);
	}

	if (p->rightChild != nil) // If p has a right child,
	{
		// we recursively call calculateHeight on it, incrementing the traversal height by one since we have gone down a level of the tree.
		calculateHeight(p->rightChild, height, traversalHeight + 1);
	}

	if (height < traversalHeight)	// If the height is less than the traversal height,
	{
		height = traversalHeight;	// than we set the height to the traversal height, as we now have a new height of the tree.
	}
}

void RBT::displayStatistics()
{
	// This method displays statistics related to the number of words in the tree and
	// insertion statistics as nodes are inserted into the RBT tree. It also
	// displays the elapsed time since the construction of the RBT tree.
	//
	// We get the current time, subtract the start time from it, and divide by 1000
	// to get the elapsed time in seconds. We calculate the elapsed time here since
	// we don't want our printing and calculating of stats to be included in the elapsed
	// time.
	double elapsedTime = (clock() - startTime) / 1000.0;

	cout << "RBT Stats:\n"; // Print out a header specifying that these are RBT stats
	cout << "Recolorings: " << numberOfRecolorings << "\n"; // Print out the number of recolorings made
	cout << "Reference Changes: " << numberOfReferenceChanges << "\n"; // Print out the number of reference changes made
	cout << "Key Comparisons: " << numberOfKeyComparisonsMade << "\n"; // Print out the number of key comparisons made
	cout << "No Fixes Needed: " << numberOfNoFixesNeeded << "\n";  // Print out the total number of insertions without any fixes needed
	cout << "Case 1 fix-ups: " << numberOfCase1Fixes << "\n"; // Print out the number of case 1 fixes performed
	cout << "Case 2 fix-ups: " << numberOfCase2Fixes << "\n"; // Print out the number of case 2 fixes performed
	cout << "Case 3 fix-ups: " << numberOfCase3Fixes << "\n"; // Print out the number of case 3 fixes performed

	unsigned int numWords, numUniqueWords; // Declare variables for the total number of words and unique words in the tree

	// Call the calculateNumWords, passing in our word counts by reference so that they are set to the proper values
	calculateNumWords(numWords, numUniqueWords);

	cout << "Words: " << numWords << "\n"; // Print out the total number of words
	cout << "Unique Words: " << numUniqueWords << "\n"; // Print out the total number of unique words
	cout << "Height: " << getHeight() << "\n"; // Print out the height of the tree
	cout << "Elapsed Time: " << elapsedTime << " seconds\n"; // Print out the elapsed time we calculated at the beginning of the method.
}