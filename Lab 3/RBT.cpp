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
	// constructor -- there's nothing to do other then initializing our nil node, and
	// making sure the root pointer is nil. We need to set up our special nil node.
	// Nil's parent, left child, and right child are all nil. It is used instead of
	// nullptr for null roots, left children, and right children to make rotations
	// and recoloring easier to implement.
	//
	nil = new node();

	strcpy(nil->word, "");	// We copy an empty string into nil's word just to give it a blank value.
	nil->color = BLACK;		// We set up nil's color to be black to follow the rules.
	nil->parent = nil->leftChild = nil->rightChild = nil; // Nil's parent and children are all nil.

	root = nil; // Since we have no nodes in the tree yet, our root is also nil.
}

RBT::~RBT()
{
	// On deconstruction, we have to delete the entire tree by
	// deleting each node in the tree except nil, one by one.
	//
	if (root != nil) // Check if the root node is not nil
	{
		// Destroying the tree is done by doing a traversal through all
		// of the nodes and their children.
		traverseDestruct(root);
	}

	// We also need to delete our special nil node.
	delete nil;

	// After the traverseDestruct method and deletion of nil, we will
	// have deleted all of the nodes we made. By setting root and nil
	// to nullptr, we make sure we don't have an old reference to the
	// now non-existent root and nil nodes.
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
	// This method left rotates certain nodes of our tree by changing
	// node children and parent pointers. This is needed to maintain
	// RBT trees as balanced binary search trees.
	//
	node* y = x->rightChild;		// We set y to be x's right child.

	x->rightChild = y->leftChild;	// x's right child becomes y's left child.
	numberOfReferenceChanges++;		// We increment our reference change counter since we just changed one.

	if (y->leftChild != nil)		// If y's left child isn't nil,
	{
		y->leftChild->parent = x;	// we update its parent to be x.
		numberOfReferenceChanges++;	// We increment our reference changes since we just made another.
	}

	y->parent = x->parent;		// We link x's parent to y.
	numberOfReferenceChanges++;	// We increment our reference change counter again!

	if (x->parent == nil) // If x has no parent, then x was the root, so y becomes the new root.
	{
		root = y;
	}
	else if (x == x->parent->leftChild) // Otherwise, the spot x used to occupy becomes y.
	{
		x->parent->leftChild = y;
	}
	else
	{
		x->parent->rightChild = y;
	}

	numberOfReferenceChanges++; // We've changed a reference in one of our if branches, so we increment our counter.

	y->leftChild = x;	// We put x on y's left, which

	x->parent = y;		// makes x's parent become y.

	numberOfReferenceChanges += 2;	// We've changed two references so we increment our counter by two.

	numberOfLeftRotations++;		// We've performed a left rotation so we increment our counter.
}

void RBT::rightRotate(node* x)
{
	// This method right rotates certain nodes of our tree by changing
	// node children and parent pointers. This is needed to maintain
	// RBT trees as balanced binary search trees. This method is symmetrical
	// to our left rotation, so we just exchange all occurrences of "left"
	// and "right."
	//
	node* y = x->leftChild;			// We set y to be x's left child.

	x->leftChild = y->rightChild;	// x's left child becomes y's right child.
	numberOfReferenceChanges++;		// We increment our reference change counter since we just changed one.

	if (y->rightChild != nil)		// If y's right child isn't nil,
	{
		y->rightChild->parent = x;	// we update its parent to be x.
		numberOfReferenceChanges++;	// We increment our reference changes since we just made another.
	}

	y->parent = x->parent;		// We link x's parent to y.
	numberOfReferenceChanges++;	// We increment our reference change counter again!

	if (x->parent == nil) // If x has no parent, then x was the root, so y becomes the new root.
	{
		root = y;
	}
	else if (x == x->parent->rightChild) // Otherwise, the spot x used to occupy becomes y.
	{
		x->parent->rightChild = y;
	}
	else
	{
		x->parent->leftChild = y;
	}

	numberOfReferenceChanges++; // We've changed a reference in one of our if branches, so we increment our counter.

	y->rightChild = x;	// We put x on y's right, which

	x->parent = y;		// makes x's parent become y.

	numberOfReferenceChanges += 2;	// We've changed two references so we increment our counter by two.

	numberOfRightRotations++;		// We've performed a right rotation so we increment our counter.
}

void RBT::insert(const char word[50])
{
	// To add a word to the tree, we need to traverse through the nodes of the tree,
	// looking for the word. If we find it, we increment the node's counter and exit;
	// otherwise, we continue traversing through the node's children, going through the
	// left child if the word is less than the node's word that we are traversing, or
	// the right child if the word is greater than the node's word we are traversing.
	// If it is not found, we just make a new node and attach it to the tree. After
	// we have inserted the node, we have to see if we need to perform a fixup of our
	// tree and fix it if needs be.
	//
	node* x = root; // x will search for where we are going to insert our node,
	node* y = nil;	// and y will lag one step behind x.

	// We define this to keep track of the compare value between our new node's word
	// and x's word so that we can use it twice below to avoid a redundant strcmp call.
	int compareValue;

	while (x != nil)	// While x isn't nil,
	{
		y = x;			// we set y to x.

		compareValue = strcmp(word, x->word); // We compare our word to x's word,

		numberOfKeyComparisonsMade++; // and increment our key comparisons since we just made one.

		if (compareValue < 0) // If word is less than x's word,
		{
			x = x->leftChild; // we go down to x's left child.
		}
		else if (compareValue > 0)	// otherwise, if word is greather than x's word,
		{
			x = x->rightChild;		// we go down to x's right child.
		}
		else
		{
			x->count++; // We increment our count by one since we found the node,

			return;		// then we are done!
		}
	}

	node* z = new node();	// We didn't find the node in the tree, so we make a new node.
	strcpy(z->word, word);	// We copy the word we are going to insert into z's word array.

	z->parent = y;				// Z's parent becomes y, as y lagged behind x.
	numberOfReferenceChanges++; // We just updated z's parent so we increment our reference change counter.

	if (y == nil)	// If the tree was empty, our new node z becomes the new root.
	{
		root = z;
		numberOfReferenceChanges++; // We increment our reference change counter since we adjusted our root.
	}
	else // Otherwise, z has to be either y's new left or right child due to the BST property.
	{
		// If z's word is less than y's word (compareValue refers to a comparison
		// between z's word and x's word which at this point is the same as y's word)
		if (compareValue < 0)
		{
			y->leftChild = z;	// y's left child becomes z.
		}
		else // otherwise,
		{
			y->rightChild = z;	// y's right child becomes z.
		}

		numberOfReferenceChanges++; // We just changed one of y's children, so we increment our reference change counter.
	}

	z->leftChild = z->rightChild = nil; // Our new node is at the bottom of the tree, so its children are nil.
	z->color = RED; // We color this node red for now.

	insertFixup(z); // We fix anything we broke due to the insert.
}

void RBT::insertFixup(node* z)
{
	// This method fixes anything that broken to an insertion of node
	// z into our tree by recoloring nodes and performing left and
	// right rotations.
	//
	if (z->parent->color == BLACK)	// If z's parent is black,
	{
		numberOfNoFixesNeeded++;	// the while loop below will never be entered, so no fixes are needed!
	}

	while (z->parent->color == RED) // While z's parent's color is red,
	{
		if (z->parent == z->parent->parent->leftChild)	// check if z's parent is its parent's left child.
		{
			node* y = z->parent->parent->rightChild;	// We set y to be z's right uncle.

			if (y->color == RED)			// If y is red,
			{
				z->parent->color = BLACK;	// We perform a case 1 where we recolor nodes.
				y->color = BLACK;
				z->parent->parent->color = RED;
				numberOfRecolorings += 3;	// We increment our recolorings counter by three since we adjusted two node colors.
				z = z->parent->parent;
				numberOfCase1Fixes++;		// We increment our case 1 counter since we just performed a case 1 fix.
			}
			else // We need to potentially perform a case 2 fix and a case 3 for sure.
			{
				if (z == z->parent->rightChild) // if z is its parent's right child, perform a case 2.
				{
					z = z->parent;			// z become z's parent,

					leftRotate(z);			// and we perform a left rotation on z.

					numberOfCase2Fixes++;	// We increment our case 2 counter since we just performed a case 2 fix.
				}

				// A case 2 leads into a case 3, so we need to perform one regardless of what happened.
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				numberOfRecolorings += 2; // We increment our recolorings counter by two since we adjusted two node colors.

				rightRotate(z->parent->parent);

				numberOfCase3Fixes++; // We increment our case 3 counter since we just performed a case 2 fix.
			}
		}
		else // This else cause is symmetrical to the if statement above, with "left" and "right" swapped.
		{
			node* y = z->parent->parent->leftChild;	// We set y to be z's left uncle.

			if (y->color == RED)				// If y is red,
			{
				z->parent->color = BLACK;		// We perform a case 1 where we recolor nodes.
				y->color = BLACK;
				z->parent->parent->color = RED;
				numberOfRecolorings += 3;		// We increment our recolorings counter by three since we adjusted two node colors.
				z = z->parent->parent;
				numberOfCase1Fixes++;			// We increment our case 1 counter since we just performed a case 1 fix.
			}
			else // We need to potentially perform a case 2 fix and a case 3 for sure.
			{
				if (z == z->parent->leftChild) // if z is its parent's left child, perform a case 2.
				{
					z = z->parent;				// z become z's parent,

					rightRotate(z);				// and we perform a right rotation on z.

					numberOfCase2Fixes++;		// We increment our case 2 counter since we just performed a case 2 fix.
				}

				// A case 2 leads into a case 3, so we need to perform one regardless of what happened.
				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				numberOfRecolorings += 2; // We increment our recolorings counter by two since we adjusted two node colors.

				leftRotate(z->parent->parent);

				numberOfCase3Fixes++; // We increment our case 3 counter since we just performed a case 2 fix.
			}
		}
	}

	root->color = BLACK; // We take care of a potential "rule 2" violation by fixing the root's color.
	numberOfRecolorings++; // Since we just recolored our root, we increment the number of recolorings by one.
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

	unsigned int height = 0; // We start at a height of zero, as a root node with no children has a height of 0.

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
	// insertion statistics as nodes are inserted into the RBT tree.
	//
	unsigned int numWords, numUniqueWords; // Declare variables for the total number of words and unique words in the tree

	// Call the calculateNumWords, passing in our word counts by reference so that they are set to the proper values
	calculateNumWords(numWords, numUniqueWords);

	cout << "RBT Stats:\n"; // Print out a header specifying that these are RBT stats
	cout << "Distinct Words: " << numUniqueWords << "\n"; // Print out the total number of unique words
	cout << "Total Words: " << numWords << "\n"; // Print out the total number of words
	cout << "Tree Height: " << getHeight() << "\n"; // Print out the height of the tree
	cout << "Key Comparisons: " << numberOfKeyComparisonsMade << "\n"; // Print out the number of key comparisons made
	cout << "Reference Changes: " << numberOfReferenceChanges << "\n"; // Print out the number of reference changes made
	cout << "Recolorings: " << numberOfRecolorings << "\n"; // Print out the number of recolorings made
	cout << "Left Rotations: " << numberOfLeftRotations << "\n"; // Print out the number of left rotations performed
	cout << "Right Rotations: " << numberOfRightRotations << "\n"; // Print out the number of right rotations performed
	cout << "No Fixes Needed: " << numberOfNoFixesNeeded << "\n";  // Print out the total number of insertions without any fixes needed
	cout << "Case 1 fix-ups: " << numberOfCase1Fixes << "\n"; // Print out the number of case 1 fixes performed
	cout << "Case 2 fix-ups: " << numberOfCase2Fixes << "\n"; // Print out the number of case 2 fixes performed
	cout << "Case 3 fix-ups: " << numberOfCase3Fixes << "\n"; // Print out the number of case 3 fixes performed
}