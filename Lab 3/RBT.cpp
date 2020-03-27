#include "RBT.h"

RBT::RBT()
{
	startTime = clock();

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
	numberOfReferenceChanges += 2;
	z->color = RED;

	insertFixup(z);
}

void RBT::insertFixup(node* z)
{
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

void RBT::print2D()
{
	print2DUtil(root, 0);
}

void RBT::print2DUtil(node* p, int space)
{
	int COUNT = 10;

	if (p == nil)
	{
		return;
	}

	space += COUNT;

	print2DUtil(p->rightChild, space);

	cout << endl;

	for (int i = COUNT; i < space; i++)
	{
		cout << " ";
	}
	cout << p->word << "\n";

	print2DUtil(p->leftChild, space);
}

void RBT::displayStatistics()
{
	cout << "RBT Stats:\n";
	cout << "Recolorings: " << numberOfRecolorings << "\n";
	cout << "Reference Changes: " << numberOfReferenceChanges << "\n";
	cout << "Key Comparisons: " << numberOfKeyComparisonsMade << "\n";
	cout << "No Fixes Needed: " << numberOfNoFixesNeeded << "\n";
	cout << "Case 1 fix-ups: " << numberOfCase1Fixes << "\n";
	cout << "Case 2 fix-ups: " << numberOfCase2Fixes << "\n";
	cout << "Case 3 fix-ups: " << numberOfCase3Fixes << "\n";
	cout << "Elapsed Time: " << (clock() - startTime) / 1000.0 << " seconds\n";
}