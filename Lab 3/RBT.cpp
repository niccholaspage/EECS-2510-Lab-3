#include "RBT.h"

RBT::RBT()
{
	nil = new node();

	strcpy(nil->word, "");
	nil->color = BLACK;
	nil->parent = nil->leftChild = nil->rightChild = nil;

	root = nil;
}

RBT::~RBT()
{
	//TODO: Traversal delete of all the nodes

	delete nil;
}

void RBT::leftRotate(node* x)
{
	// The pseudocode for this can be found in Lecture 11, slide 18. Comments are provided.
	node* y = x->rightChild;

	x->rightChild = y->leftChild;

	if (y->leftChild != nil)
	{
		y->leftChild->parent = x;
	}

	y->parent = x->parent;

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

	y->leftChild = x;

	x->parent = y;
}

void RBT::rightRotate(node* x)
{
	node* y = x->leftChild;

	x->leftChild = y->rightChild;

	if (y->rightChild != nil)
	{
		y->rightChild->parent = x;
	}

	y->parent = x->parent;

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

	y->rightChild = x;

	x->parent = y;
}

void RBT::insert(const char word[50])
{
	node* y = nil;
	node* x = root;

	while (x != nil)
	{
		y = x;

		int compareValue = strcmp(word, x->word);

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

	if (y == nil)
	{
		root = z;
	}
	else
	{
		int compareValue = strcmp(z->word, y->word);

		if (compareValue < 0)
		{
			y->leftChild = z;
		}
		else
		{
			y->rightChild = z;
		}
	}

	z->leftChild = nil;
	z->rightChild = nil;
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
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->rightChild)
				{
					z = z->parent;

					leftRotate(z);
				}

				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				rightRotate(z->parent->parent);
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
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->leftChild)
				{
					z = z->parent;

					rightRotate(z);
				}

				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				leftRotate(z->parent->parent);
			}
		}
	}

	root->color = BLACK;
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