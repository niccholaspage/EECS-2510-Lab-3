//==============================================================================================
// File: BST.cpp - Binary search tree implementation
// c.f.: BST.h
//
// This class implements a binary search tree, filled with nodes as declared in BST.h.  
// Each node contains a word, the number of times the word has appeared, and pointers to 
// the left and right child of the node. This tree does not perform any rotations or fix-ups
// of the nodes inserted into it, meaning that no rebalancing is done.
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Mar 31, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

#include "BST.h"

BST::BST()
{
	// constructor -- there's nothing to do other then setting our start
	// time and making sure the root pointer is a nullptr. The default value
	// is in the header, but I am doing it again for redundancy and clarity.
	//
	startTime = clock(); // We set the starting time position to the current time.

	root = nullptr; // When a new binary search tree is constructed, there is no root node.
}

BST::~BST()
{
	// On deconstruction, we have to delete the entire tree by
	// deleting each node in the tree, one by one.
	//
	if (root == nullptr) // Check if the root node is null
	{
		return; // Return because there are no nodes to delete!
	}


	// Destroying the tree is done by doing a traversal through all
	// of the nodes and their children.
	traverseDestruct(root);

	// After the traverseDestruct method, we will have deleted
	// all of the nodes. By setting root to nullptr, we make sure
	// we don't have an old reference to the now non-existent root
	// node.
	root = nullptr;
}

void BST::traverseDestruct(node* p) {
	// This recursive method can be called on a given node to delete
	// its children and itself. Doing this recursively ensures that
	// every node will get deleted as long as this method is initially
	// called on the root node.
	//
	if (p->leftChild != nullptr) // Check if the left child of the node is not null
	{
		traverseDestruct(p->leftChild); // Since the node does have a left child, we will call the traverseDestruct method on the node
	}

	if (p->rightChild != nullptr) // Check if the right child is not null
	{
		traverseDestruct(p->rightChild); // Since the node does have a right child, we will call the traverseDestruct method on the node
	}

	// Now that we have properly deleted the children of the given node,
	// we are safe to delete the node since we have already taken care of
	// its children.
	//
	delete p;
}

void BST::insert(const char word[50])
{
	// To a add a word to the tree, we need to traverse through the nodes of the tree,
	// looking for the word. If we find it, we increment the node's counter and exit;
	// otherwise, we continue traversing through the node's children, going through the
	// left child if the word is less than the node's word that we are traversing, or
	// the right child if the word is greater than the node's word we are traversing.
	// If it is not found, we just make a new node and attach it to the tree.
	//
	node* p = root; // traverses the tree, starting at the root node
	node* q = nullptr; // lags one step behind p; used to update q's child to a new node if it was made.

	// We define this to keep track of the compare value between our new node's word
	// and p's word so that we can use it twice below to avoid a redundant strcmp call.
	int compareValue;

	while (p != nullptr) // as long as there are more nodes:
	{
		q = p; // Set our lagging pointer to q

		compareValue = strcmp(word, p->word); // Compare the word we are inserting to p's word

		numberOfKeyComparisonsMade++; // Increment our number of key comparisons as we just made one

		if (compareValue < 0) // Check if the word we are inserting is less than p's word
		{
			p = p->leftChild; // If so, p gets set to old p's left child, so that we can check it.
		}
		else if (compareValue > 0) // Otherwise, check if our word is greater than p's word.
		{
			p = p->rightChild; // If so, p gets set to old p's right child, so that we can check it.
		}
		else // Since the word was not less than or greater than p's word, there is only one case remaining - it equals p's word
		{
			p->count++; // Since a node with the word already exists, we just increment the node's count by one.

			return; // Since we don't need to insert any new nodes into the tree, we can just stop here. We are done!
		}
	}

	// Since we've gotten here, we weren't able to find a node
	// with the word already in the tree, so lets build a new
	// node to store the word.
	//
	node* newNode = new node(); // Construct a new node

	strcpy(newNode->word, word); // Copy the contents of the word array we are inserting into the new node's word array
	newNode->count = 1; // Redundantly set the count in the new node to 1
	newNode->leftChild = nullptr; // Since new nodes are added as leaves, left and right child pointers are null
	newNode->rightChild = nullptr;

	if (q == nullptr) // If q is nullptr, that means that the new node has no parent, so it is the root!
	{
		root = newNode; // Since the new node must be the root, we set the root of the tree to the new node.
	}
	else
	{
		// If our new node's word is less than q's word (compareValue refers to a comparison
		// between the new node's word and q's word which at this point is the same as p's word)
		if (compareValue < 0)
		{
			q->leftChild = newNode; // If so, we set q's left child to the new node.
		}
		else // Otherwise, the new node's word must be greater than q,
		{
			q->rightChild = newNode; // so we set q's right child to the new node.
		}
	}

	numberOfReferenceChanges++; // We have changed a single reference above so we increment our reference change coutner
}

void BST::list()
{
	// This method traverses through the entire tree and prints out the nodes in a nicely formatted list
	// with indexes and the node's word and count.
	//
	if (root == nullptr) // If the root is nullptr, than there is nothing in the tree,
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

void BST::list(int& index, node* p)
{
	// This method does a recursive traversal through all of the nodes in the tree
	// and prints out their word and count with an index parameter. The index
	// parameter is passed by reference so that no matter which recursive call
	// of this method we are in, the correct index will always be printed out.
	// Through talking with Dr. Thomas, I learned that the index variable could
	// have been a class variable instead. I enjoy this method because it does
	// not require any cleanup or setup of a class variable.
	//
	if (p->leftChild != nullptr)   // If the left child of the node is not null,
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

	if (p->rightChild != nullptr) // If the right child of the node is not null,
	{
		// we print out a comma to separate the node we already printed from
		// the right child we are about to print.
		cout << ", ";

		list(index, p->rightChild); // we now print out the right child node as well, recursively taking care of its children.
	}
}

void BST::calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords)
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

void BST::calculateNumWords(node* p, unsigned int& numWords, unsigned int& numUniqueWords)
{
	// This method does a recursive traversal through all of the nodes in the tree
	// and increments the references to the number of words and unique words through
	// each call.
	//
	if (p->leftChild != nullptr) // If the left child of the node is not null,
	{
		calculateNumWords(p->leftChild, numWords, numUniqueWords); // we call the method on the left child of the node.
	}

	if (p->rightChild != nullptr) // If the right child of the node is not null,
	{
		calculateNumWords(p->rightChild, numWords, numUniqueWords); // we call the method on the left child of the node.
	}

	numWords += p->count;	// We increment the number of words by the node's count,
	numUniqueWords += 1;	// and the number of unique words by one, as each node represents a unique word.
}

unsigned int BST::getHeight()
{
	// This method returns the height of the tree. In this program, the height is NOT
	// the number of child pointer links required to get from the root to the deepest leaf. Instead,
	// it is the amount of levels of a tree. For example, a tree with no nodes has a height of zero,
	// and a tree with only a root node has a height of 1.
	//
	if (root == nullptr) // If there is no root,
	{
		return 0; // we have no nodes in the tree, so the height is 0.
	}

	unsigned int height = 0; // We start at a height of zero, as a root node with no children has a height of 0.

	// We call our calculate height method on the root node, which will recursively go through
	// each node in the tree, and properly update our height variable with the height of the entire tree.
	calculateHeight(root, height, height);

	return height;
}

void BST::calculateHeight(node* p, unsigned int& height, unsigned int traversalHeight)
{
	// This method does a recursive traversal of the given node and gets called on every node
	// underneath it. As it traverses through each node, the traversal height gets incremented
	// by one, and the height gets changed to the traversal height if the traversal height is greater
	// than it.
	//
	if (p->leftChild != nullptr) // If p has a left child,
	{
		// we recursively call calculateHeight on it, incrementing the traversal height by one since we have gone down a level of the tree.
		calculateHeight(p->leftChild, height, traversalHeight + 1);
	}

	if (p->rightChild != nullptr) // If p has a right child,
	{
		// we recursively call calculateHeight on it, incrementing the traversal height by one since we have gone down a level of the tree.
		calculateHeight(p->rightChild, height, traversalHeight + 1);
	}

	if (height < traversalHeight)	// If the height is less than the traversal height,
	{
		height = traversalHeight;	// than we set the height to the traversal height, as we now have a new height of the tree.
	}
}

void BST::displayStatistics()
{
	// This method displays statistics related to the number of words in the tree and
	// insertion statistics as nodes are inserted into the binary search tree. It also
	// displays the elapsed time since the construction of the binary search tree.
	//
	// We get the current time, subtract the start time from it, and divide by 1000
	// to get the elapsed time in seconds. We calculate the elapsed time here since
	// we don't want our printing and calculating of stats to be included in the elapsed
	// time.
	double elapsedTime = (clock() - startTime) / 1000.0;

	cout << "BST Stats:\n"; // Print out a header specifying that these are BST stats
	cout << "Reference Changes: " << numberOfReferenceChanges << "\n"; // Print out the total number of reference changes made
	cout << "Key Comparisons: " << numberOfKeyComparisonsMade << "\n"; // Print out the total number of key comparisons made

	unsigned int numWords, numUniqueWords; // Declare variables for the total number of words and unique words in the tree

	// Call the calculateNumWords, passing in our word counts by reference so that they are set to the proper values
	calculateNumWords(numWords, numUniqueWords);

	cout << "Words: " << numWords << "\n"; // Print out the total number of words
	cout << "Unique Words: " << numUniqueWords << "\n"; // Print out the total number of unique words
	cout << "Height: " << getHeight() << "\n"; // Print out the height of the tree
	cout << "Elapsed Time: " << elapsedTime << " seconds\n"; // Print out the elapsed time we calculated at the beginning of the method.
}