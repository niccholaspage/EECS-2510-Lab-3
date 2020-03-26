#include "AVL.h"

AVL::AVL()
{
	// constructor -- there's nothing to do other then making sure
	// the root pointer is a nullptr. The default value is in the
	// header, but I am doing it again for redundancy and clarity.
	//
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
	// TODO: Place comments. This pseudocode is found in slide 51 of Lecture 12
	node* y;
	node* a, * b, * f;
	node* p, * q;
	node* c, * cl, * cr;
	int d;

	if (root == nullptr)
	{
		y = new node();
		strcpy(y->word, word);
		y->leftChild = y->rightChild = nullptr;
		y->balanceFactor = 0;
		root = y;
		return;
	}

	f = q = nullptr;
	a = p = root;

	while (p != nullptr)
	{
		int compareValue = strcmp(word, p->word);

		if (compareValue == 0)
		{
			p->count++;

			return;
		}

		if (p->balanceFactor != 0)
		{
			a = p;
			f = q;
		}

		q = p;
		p = compareValue < 0 ? p->leftChild : p->rightChild; // Potentially change this to a normal if statement?
	}

	y = new node();
	strcpy(y->word, word);
	y->leftChild = y->rightChild = nullptr;
	y->balanceFactor = 0;

	if (strcmp(word, q->word) < 0)
	{
		q->leftChild = y;
	}
	else
	{
		q->rightChild = y;
	}

	if (strcmp(word, a->word) > 0)
	{
		b = p = a->rightChild;

		d = -1;
	}
	else
	{
		b = p = a->leftChild;

		d = +1;
	}

	while (p != y)
	{
		if (strcmp(word, p->word) > 0)
		{
			p->balanceFactor = -1;
			p = p->rightChild;
		}
		else
		{
			p->balanceFactor = +1;
			p = p->leftChild;
		}
	}

	if (a->balanceFactor == 0)
	{
		a->balanceFactor = d;
		return;
	}

	if (a->balanceFactor == -d)
	{
		a->balanceFactor = 0;
		return;
	}

	if (d == +1)
	{
		if (b->balanceFactor == +1)
		{
			cout << "Slide 57: Attempting LL rotation:\n";
			cout << "Node A: " << a->word << endl;
			cout << "Node B: " << b->word << endl;

			b->rightChild = a;
			a->leftChild = nullptr;
			a->balanceFactor = b->balanceFactor = 0;
		}
		else
		{
			print2D();
			cout << "Slide 57: Attempting LR rotation\n";
			c = b->rightChild;
			cl = c->leftChild;
			cr = c->rightChild;

			cout << "Node A: " << a->word << ", balance factor: " << a->balanceFactor << endl;
			cout << "Node B: " << b->word << ", balance factor: " << b->balanceFactor << endl;
			cout << "Node C: " << c->word << ", balance factor: " << c->balanceFactor << endl;

			c->leftChild = b;
			c->rightChild = a;
			b->rightChild = cl;
			a->leftChild = cr;

			switch (c->balanceFactor)
			{
			case 0: a->balanceFactor = b->balanceFactor = 0; break;
			case -1: b->balanceFactor = +1; a->balanceFactor = c->balanceFactor = 0; break;
			case 1: a->balanceFactor = -1; b->balanceFactor = c->balanceFactor = 0; break;
			}

			c->balanceFactor = 0;
			b = c;
		}
	}
	else
	{
		if (b->balanceFactor == -1)
		{
			cout << "Slide 57: Attempting RR rotation:\n";
			cout << "Node A: " << a->word << endl;
			cout << "Node B: " << b->word << endl;

			b->leftChild = a;
			a->rightChild = nullptr;
			a->balanceFactor = b->balanceFactor = 0;
		}
		else
		{
			cout << "Attempting RL rotation\n";
			c = b->leftChild;
			cl = c->leftChild;
			cr = c->rightChild;

			cout << "Node A: " << a->word << ", balance factor: " << a->balanceFactor << endl;
			cout << "Node B: " << b->word << ", balance factor: " << b->balanceFactor << endl;
			cout << "Node C: " << c->word << ", balance factor: " << c->balanceFactor << endl;

			c->rightChild = b;
			c->leftChild = a;
			b->leftChild = cr;
			a->rightChild = cl;

			switch (c->balanceFactor)
			{
			case 0: a->balanceFactor = b->balanceFactor = 0; break;
			case -1: a->balanceFactor = +1; b->balanceFactor = c->balanceFactor = 0; break;
			case 1: b->balanceFactor = -1; a->balanceFactor = c->balanceFactor = 0; break;
			}

			c->balanceFactor = 0;
			b = c;
		}
	}

	if (f == nullptr)
	{
		root = b;
		return;
	}

	if (a == f->leftChild)
	{
		f->leftChild = b;
		return;
	}

	if (a == f->rightChild)
	{
		f->rightChild = b;
		return;
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

void AVL::print2D()
{
	print2DUtil(root, 0);
}

void AVL::print2DUtil(node* p, int space)
{
	int COUNT = 10;

	if (p == nullptr)
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
	cout << p->word << " (" << p->balanceFactor << ")" << "\n";

	print2DUtil(p->leftChild, space);
}

void AVL::displayStatistics()
{
	// TODO: Implement method
}