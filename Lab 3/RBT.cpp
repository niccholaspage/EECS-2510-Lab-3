#include "RBT.h"

RBT::RBT()
{
	nil = new node();

	strcpy(nil->word, "");
	nil->color = BLACK;
	nil->parent = nil->leftChild = nil->rightChild = nil;

	root = nil;
}

RBT::RBT()
{
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