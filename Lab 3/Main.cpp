#include <iostream>

#include "BST.h"
#include "RBT.h"
#include "SkipList.h"

using namespace std;

int main()
{
	cout << "Hello there!\n";

	RBT* tree = new RBT();

	tree->insert("b");
	tree->insert("a");
	tree->insert("c");
	tree->insert("d");
	tree->insert("e");
	tree->insert("f");
	tree->insert("f");
	tree->insert("q");
	tree->insert("z");
	tree->insert("k");
	tree->insert("k");
	tree->insert("k");
	tree->insert("v");
	tree->insert("z");
	tree->insert("u");
	tree->insert("t");
	tree->insert("h");
	tree->insert("s");
	tree->insert("y");
	tree->insert("x");
	tree->insert("w");
	tree->insert("m");
	tree->insert("p");
	tree->insert("n");
	tree->insert("l");
	tree->insert("o");
	tree->insert("z");
	tree->insert("j");
	tree->insert("s");
	tree->insert("i");
	tree->insert("r");
	tree->insert("o");
	tree->insert("g");

	tree->print2D();

	delete tree;

	return 0;
}