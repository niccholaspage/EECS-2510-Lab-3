#include <iostream>
#include <fstream>

#include "AVL.h"
#include "BST.h"
#include "RBT.h"
#include "SkipList.h"

using namespace std;

void runTests()
{
	char c; RBT* RBT_T = new RBT();
	// instantiate each of the trees
	AVL* AVL_T = new AVL();
	BST* BST_T = new BST();
	SkipList* SL = new SkipList();
	// and the skip list
	char chari[50]; // assumes no word is longer than 49 characters
	memset(chari, 0, 50); // zero the word buffer
	int iPtr;
	ifstream inFile;

	for (int pass = 0; pass < 6; pass++) {
		bool IsDelimiter = false, WasDelimiter = false;
		inFile.open("I:\\Shakespeare.txt", ios::binary);
		if (inFile.fail())
		{
			cout << "Unable to open input file\n\n" << "Program Exiting\n\nPress ENTER to exit\n";
			cin.get(c);
			exit(1);
		}
		iPtr = 0;
		inFile.get(c); // priming read
		while (!inFile.eof())
		{
			IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == ',' || c == '.' || c == '\'' ||
				c == ':' || c == ';' || c == '"' || c == '?' || c == '!' || c == '-' ||
				c == '(' || c == ')' || c == '[' || c == ']' || c == '_' || c == '*' || c == 9);
			if (IsDelimiter && !WasDelimiter)   // if THIS character IS a delimiter, and the // last one WASN’T, it’s the end of a word
			{
				WasDelimiter = true;
				if (pass == 2) RBT_T->insert(chari);  // insert this word in the RBT
				else if (pass == 3) AVL_T->insert(chari);  // insert it in the AVL Tree
				else if (pass == 4) BST_T->insert(chari);  // insert it in the BST
				else if (pass == 5)    SL->insert(chari);  // insert it in the skip list
				memset(chari, 0, 50); // zero the word buffer
				iPtr = 0;
			}
			else if (!IsDelimiter)  chari[iPtr++] = c;  // if this isn’t a delimiter, keep going
			WasDelimiter = IsDelimiter;   // forthe NEXT iteration
			inFile.get(c);
		}
		inFile.close();
		// If the file doesn't end with a delimiter, it will leave the last word unprocessed.
		// Insert once more, and display the statistics for this structure...
		//
		if (pass == 2) { if (strlen(chari)) RBT_T->insert(chari); RBT_T->displayStatistics(); } // RBT
		else if (pass == 3) { if (strlen(chari)) AVL_T->insert(chari);  AVL_T->displayStatistics(); } // AVL
		else if (pass == 4) { if (strlen(chari)) BST_T->insert(chari);  BST_T->displayStatistics(); } // BST
		else if (pass == 5) { if (strlen(chari))    SL->insert(chari);     SL->displayStatistics(); } // skip list
	}
}

int main()
{
	cout << "Hello there!\n";

	AVL* tree = new AVL();

	tree->insert("mar");
	tree->insert("may");
	tree->insert("nov");
	tree->insert("aug");
	tree->insert("apr");
	tree->insert("jan");
	tree->insert("dec");
	tree->insert("jul");
	tree->insert("feb");
	tree->insert("jun");
	tree->insert("oct");
	tree->insert("sep");

	/*tree->insert("b");
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
	tree->insert("g");*/

	tree->list();

	tree->print2D();

	delete tree;

	return 0;
}