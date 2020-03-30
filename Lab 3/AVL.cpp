//==============================================================================================
// File: AVL.cpp - Adelson-Velsky and Landis tree implementation
// c.f.: AVL.h
//
// This class implements an AVL tree, filled with nodes as declared in AVL.h.  
// Each node contains a word, the number of times the word has appeared, pointers to 
// the left and right child of the node, and the balance factor of the node. This tree
// performs different types of rotations based on the balance factors of nodes in the tree
// as nodes are inserted into it.
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Mar 31, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

#include "AVL.h"


AVL::AVL()
{
	// constructor -- there's nothing to do other then setting our start
	// time and making sure the root pointer is a nullptr. The default value
	// is in the header, but I am doing it again for redundancy and clarity.
	//
	startTime = clock(); // We set the starting time position to the current time.

	root = nullptr; // When a new binary search tree is constructed, there is no root node.
}

AVL::~AVL()
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

void AVL::traverseDestruct(node* p) {
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

void AVL::insert(const char word[50])
{
	// To add a word to the tree, we need to traverse through the nodes of the tree,
	// looking for the word. If we find it, we increment the node's counter and exit;
	// otherwise, we continue traversing through the node's children, going through the
	// left child if the word is less than the node's word that we are traversing, or
	// the right child if the word is greater than the node's word we are traversing.
	// If it is not found, we just make a new node and attach it to the tree. After
	// we have inserted the node, we check if we have an unacceptable imbalance in the
	// tree and fix it if needs be.
	//
	node* y;				// The new node we will be inserting
	node* a, * b, * f;		// see below...
	node* p, * q;			// ...
	node* c, * cl, * cr;	// ... for description
	int d;					// displacement, used to adjust balance factors

	if (root == nullptr)	// If the tree is empty, then we just need to make a root node and we're done!
	{
		y = new node();			// construct a root node
		strcpy(y->word, word);	// copy the word we are inserting into y's word array
		y->leftChild = y->rightChild = nullptr;	// since this new node is a leaf, it has no children
		y->balanceFactor = 0; // a leaf has no children so it has to be balanced!
		root = y; // root was null, so Y is the new root
		numberOfReferenceChanges++;		// increment the number of reference changes since we changed the root
		numberOfNoRotationsNeeded++;	// increment the number of no rotations needed since we didn't perform a rotation
		return;
	}

	// We now need to locate the insertion point for x.
	// Pointer p will scan through the tree until it falls off the bottom.
	// Pointer q is p's parent, lagging on step behind p.
	// The new node y will be added as a left or right child of q.
	// A is the last parent above y with a balance factor of -/+ 1 before the insert.
	// Pointer f is a's parent, lagging one step behind A.
	f = q = nullptr;
	a = p = root;

	while (p != nullptr) // lets search the tree for our insertion point
	{
		int compareValue = strcmp(word, p->word); // compare the word we are inserting to p's word
		numberOfKeyComparisonsMade++; // increment the number of key comparisons since we just made one

		if (compareValue == 0)	// if word equals p->word...
		{
			p->count++;			// increment p's count by one,

			return;				// and return because we are done here!
		}

		if (p->balanceFactor != 0)	// remember the last place we saw
		{
			a = p;					// a non-zero balance factor
			f = q;					// and its parent
		}

		q = p; // we now bring q up to where p is,

		// and advance p to either its left or right child based on if the word is less than or greater than p's word.
		p = compareValue < 0 ? p->leftChild : p->rightChild;
	}

	// At this point, p is nullptr, but q points at the last node where x
	// belongs (either as q's left child or right child, and q points to an existing leaf)
	//
	y = new node();			// construct a new node,
	strcpy(y->word, word);	// copy the word we are inserting into y's word array
	y->leftChild = y->rightChild = nullptr;	// since this new node is a leaf, it has no children
	y->balanceFactor = 0; // a leaf has no children so it has to be balanced!

	// Pointer y will be q's new left or right child based on a comparison between its word and q's word
	if (strcmp(word, q->word) < 0)
	{
		q->leftChild = y;
	}
	else
	{
		q->rightChild = y;
	}

	numberOfKeyComparisonsMade++;	// increment the number of key comparisons since we just made one,
	numberOfReferenceChanges++;		// and increment the number of reference changes since we changed q's left or right child.

	// At this point, we have just done the same BST insert that we've done before. Now, we do the AVL
	// specific operations to detect and fix an imbalance if we have one.

	// Now we need to adjust the balance factors from pointers a to q.
	// A was the last ancestor with a balance factor of -/+ 1, (or is still the root, because we never found
	// a balance factor of -/+ 1), so all nodes between a and q must have a balance factor of 0,
	// and will therefore become -/+ 1.
	//

	// If x is inserted into the left subtree of a, then displacement becomes +1.
	// Displacement becoming -1 means we inserted x in the right subtree of a.
	if (strcmp(word, a->word) > 0) // compare word and a's word so we can determine which way the displacement is
	{
		b = p = a->rightChild;	// b becomes a's right child

		d = -1;
	}
	else
	{
		b = p = a->leftChild;	// b become's a's left child

		d = +1;
	}

	numberOfKeyComparisonsMade++; // increment number of key comparisons since we just compared word and a's word

	while (p != y)	// pointer p is now one node below a. We adjust from here
	{				// to the insertion point, and don't touch our new node (y).
		if (strcmp(word, p->word) > 0)	// we compare the word to p's word,
		{
			p->balanceFactor = -1;		// and adjust the balance factor to -1,
			p = p->rightChild;			// and move forward,
		}
		else
		{
			p->balanceFactor = +1;		// or adjust the balance factor to +1,
			p = p->leftChild;			// and move forward.
		}

		numberOfKeyComparisonsMade++;	// we increment the number of key comparisons made since we compared the word to p's word,
		numberOfBalanceFactorChanges++;	// and we increment our balance factor changes since we just changed p's balance factor.
	}

	// Now we check the balance factor at a and see if we just pushed the tree into balance,
	// into an "unacceptable imbalance," or if it is still balanced enough.

	if (a->balanceFactor == 0)	// The tree was completely balanced before the insert.
	{							// The insert pushed it to a (slight) acceptable imbalance.
		a->balanceFactor = d;	// We set the balance factor to either -/+ 1, based on the displacement direction.
		numberOfBalanceFactorChanges++; // We increment the balance factor changes since we just adjusted a's balance factor.
		numberOfNoRotationsNeeded++;	// Since we are about to return, we haven't made any rotations!
		return;							// This is close enough to live with so we exit now.
	}

	if (a->balanceFactor == -d) // If the tree had a slight imbalance the other way,
	{							// then did the insertion throw the tree into complete balance?
		a->balanceFactor = 0;	// If it did, then we set the balance factor to zero!
		numberOfBalanceFactorChanges++; // We increment the balance factor changes since we adjusted a's balance factor.
		numberOfNoRotationsNeeded++;	// Since we are about to return, we haven't made any rotations!
		return;
	}

	// If we took neither of the two returns above, then the tree was acceptably imbalanced, and is now
	// unacceptably imbalanced, so we have to detrmine which rotation type we need to apply.

	if (d == +1)	// this is a left imbalance (the left subtree is too tall)
	{				// Is it a LL or LR rotation?
		if (b->balanceFactor == +1) // LL rotation
		{
			// At this point, we need to change the child pointers of nodes
			// a and b to reflect the rotation.

			// cout << "Slide 57: Attempting LL rotation:\n";
			// cout << "Node A: " << a->word << endl;
			// cout << "Node B: " << b->word << endl;

			a->leftChild = b->rightChild;	// We set A's left child to be b's right child,
			b->rightChild = a;				// and set b's right child to be a.
			numberOfReferenceChanges += 2;	// since we've changed two references, we increment the count by two.
			a->balanceFactor = b->balanceFactor = 0;	// since nodes a and b are both balanced now, we set their balance factors to 0,
			numberOfBalanceFactorChanges += 2;			// and increment the balance factor changes by two.

			numberOfLeftLeftRotations++;				// since we've performed a left left rotation, we increment the count by one.
		}
		else // LR rotation: we have three different cases to handle in terms of the balance factors
		{
			// At this point, we need to adjust the child pointers of nodes
			// a, b, and c to reflect the new post-rotation structure.
			// cout << "Slide 57: Attempting LR rotation\n";
			c = b->rightChild; // c is b's right child
			cl = c->leftChild; // cl and cr are c's left and right children.
			cr = c->rightChild;

			// cout << "Node A: " << a->word << ", balance factor: " << a->balanceFactor << endl;
			// cout << "Node B: " << b->word << ", balance factor: " << b->balanceFactor << endl;
			// cout << "Node C: " << c->word << ", balance factor: " << c->balanceFactor << endl;

			c->leftChild = b;	// We set c's left child to be b,
			c->rightChild = a;	// set c's right child to be a,
			b->rightChild = cl;	// set b's right child to be c's old left child,
			a->leftChild = cr;	// and set a's left child to be c's old right child.

			numberOfReferenceChanges += 4;	// since we've changed four references, we increment this count by 4.

			switch (c->balanceFactor) // at this point, we need to adjust a and b's balance factors based on c's current balance factor.
			{
			case 0: a->balanceFactor = b->balanceFactor = 0; break;			// if c's balance factor is 0, than a and b are now both balanced.
			case -1: b->balanceFactor = +1; a->balanceFactor = 0; break;	// if c's balance factor is -1, b has a balance factor of 1, and a is completely balanced.
			case 1: a->balanceFactor = -1; b->balanceFactor = 0; break;		// if c's balance factor is 1, a has a balance factor of -1, and b is completely balanced.
			}

			c->balanceFactor = 0;	// regardless of what's happened, c is now balanced.
			b = c;					// b is the root of the now-rebalanced subtree.

			numberOfBalanceFactorChanges += 3;	// we've changed a, b, and c's balance factors, so we increment this count by three.
			numberOfLeftRightRotations++;		// since we've performed a left right rotation, we increment the count by one.
		}
	}
	else // d = -1 at this point, so this is a right imbalance.
	{
		if (b->balanceFactor == -1)
		{
			// At this point, we need to change the child pointers of nodes
			// a and b to reflect the rotation.

			// cout << "Slide 57: Attempting RR rotation:\n";
			// cout << "Node A: " << a->word << ", balance factor: " << a->balanceFactor << endl;
			// cout << "Node B: " << b->word << ", balance factor: " << b->balanceFactor << endl;

			a->rightChild = b->leftChild;	// We set a's right child to be b's right child,
			b->leftChild = a;				// and set b's left child to be a.
			numberOfReferenceChanges += 2;	// since we've changed two references, we increment the count by two.
			a->balanceFactor = b->balanceFactor = 0;	// since nodes a and b are both balanced now, we set their balance factors to 0,
			numberOfBalanceFactorChanges += 2;			// and increment the balance factor changes by two.

			numberOfRightRightRotations++;				// since we've performed a left left rotation, we increment the count by one.
		}
		else // RL rotation: we have three different cases to handle in terms of the balance factors
		{
			// At this point, we need to adjust the child pointers of nodes
			// a, b, and c to reflect the new post-rotation structure.
			// cout << "Attempting RL rotation\n";
			c = b->leftChild;	// c is b's left child
			cl = c->leftChild;	// cl and cr are c's left and right children.
			cr = c->rightChild;

			// cout << "Node A: " << a->word << ", balance factor: " << a->balanceFactor << endl;
			// cout << "Node B: " << b->word << ", balance factor: " << b->balanceFactor << endl;
			// cout << "Node C: " << c->word << ", balance factor: " << c->balanceFactor << endl;

			c->rightChild = b;	// We set c's right child to be b,
			c->leftChild = a;	// set c's left child to be a,
			b->leftChild = cr;	// set b's left child to be c's old right child,
			a->rightChild = cl;	// and set a's right child to be c's old left child.

			numberOfReferenceChanges += 4;	// since we've changed four references, we increment this count by 4.

			switch (c->balanceFactor) // at this point, we need to adjust a and b's balance factors based on c's current balance factor.
			{
			case 0: a->balanceFactor = b->balanceFactor = 0; break;			// if c's balance factor is 0, than a and b are now both balanced.
			case -1: a->balanceFactor = +1; b->balanceFactor = 0; break;	// if c's balance factor is -1, a has a balance factor of 1, and b is completely balanced.
			case 1: b->balanceFactor = -1; a->balanceFactor = 0; break;		// if c's balance factor is 1, b has a balance factor of -1, and a is completely balanced.
			}

			c->balanceFactor = 0;	// regardless of what's happened, c is now balanced.
			b = c;					// b is the root of the now-rebalanced subtree.

			numberOfBalanceFactorChanges += 3;	// we've changed a, b, and c's balance factors, so we increment this count by three.

			numberOfRightLeftRotations++;		// since we've performed a right left rotation, we increment the count by one.
		}
	}

	// Now we finish up. The subtree rooted at a has been rebalanced, and needs to be
	// the new child of f. The original subtree of f had root a.

	// Did we rebalance the whole tree's root?
	if (f == nullptr)
	{
		root = b; // We set the root to pointer b - we are done.
		numberOfReferenceChanges++; // increment the number of reference changes since we just changed the root,
		return;						// and exit.
	}

	/// The root of what we rebalanced was a; now it's b. If the subtree we rebalanced was left of f,
	// then b needs to be left of f; if a was right of f, then b now needs to be right of f.
	//
	if (a == f->leftChild)
	{
		f->leftChild = b;
		numberOfReferenceChanges++; // we increment the number of reference changes since we just changed f's left child,
		return;						// and exit.
	}

	if (a == f->rightChild)
	{
		f->rightChild = b;
		numberOfReferenceChanges++; // we increment the number of reference changes since we just changed f's left child,
		return;						// and exit.
	}

	cout << "We should never be here!\n";
}

void AVL::list()
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

void AVL::list(int& index, node* p)
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

void AVL::calculateNumWords(unsigned int& numWords, unsigned int& numUniqueWords)
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

void AVL::calculateNumWords(node* p, unsigned int& numWords, unsigned int& numUniqueWords)
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

unsigned int AVL::getHeight()
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

void AVL::calculateHeight(node* p, unsigned int& height, unsigned int traversalHeight)
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

void AVL::displayStatistics()
{
	// This method displays statistics related to the number of words in the tree and
	// insertion statistics as nodes are inserted into the AVL tree. It also
	// displays the elapsed time since the construction of the AVL tree.
	//
	// We get the current time, subtract the start time from it, and divide by 1000
	// to get the elapsed time in seconds. We calculate the elapsed time here since
	// we don't want our printing and calculating of stats to be included in the elapsed
	// time.
	double elapsedTime = (clock() - startTime) / 1000.0;

	cout << "AVL Stats:\n"; // Print out a header specifying that these are AVL stats
	cout << "Balance Factor Changes: " << numberOfBalanceFactorChanges << "\n"; // Print out the total number of balance factor changes
	cout << "Reference Changes: " << numberOfReferenceChanges << "\n"; // Print out the total number of reference changes made
	cout << "Key Comparisons: " << numberOfKeyComparisonsMade << "\n"; // Print out the total number of key comparisons made
	cout << "No Rotations Needed: " << numberOfNoRotationsNeeded << "\n"; // Print out the total number of insertions without any rotations needed
	cout << "LL Rotations: " << numberOfLeftLeftRotations << "\n";		// Print out the total number of left left rotations made
	cout << "LR Rotations: " << numberOfLeftRightRotations << "\n";		// Print out the total number of left right rotations made
	cout << "RR Rotations: " << numberOfRightRightRotations << "\n";	// Print out the total number of right right rotations made
	cout << "RL Rotations: " << numberOfRightLeftRotations << "\n";		// Print out the total number of right left rotations made

	unsigned int numWords, numUniqueWords; // Declare variables for the total number of words and unique words in the tree

	// Call the calculateNumWords, passing in our word counts by reference so that they are set to the proper values
	calculateNumWords(numWords, numUniqueWords);

	cout << "Words: " << numWords << "\n"; // Print out the total number of words
	cout << "Unique Words: " << numUniqueWords << "\n"; // Print out the total number of unique words
	cout << "Height: " << getHeight() << "\n"; // Print out the height of the tree
	cout << "Elapsed Time: " << elapsedTime << " seconds\n"; // Print out the elapsed time we calculated at the beginning of the method.
}