//==============================================================================================
// Main.cpp - BST / AVL / RBT / SL Comparison Program
//
// This program implements the AVL, BST, RBT, and Skip list datatypes and benchmarks them
// by timing how long inserting each word of an input file takes. Once each datatype has finished,
// stats are printed out that are relevant to each datatype, including how many words and unique
// words are in the datatype, and specific stats for each type, like the number of left-right
// rotations of an AVL tree.
//
// Author:     Nicholas Nassar, University of Toledo
// Class:      EECS 2510-001 Non-Linear Data Structures, Spring 2020
// Instructor: Dr.Thomas
// Date:       Mar 31, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

#include <iostream>
#include <fstream>

#include "AVL.h"
#include "BST.h"
#include "RBT.h"
#include "SkipList.h"

using namespace std;

void runTests()
{
	// This method utilizes the file parser code given by Dr. Thomas in listing 2
	// of the lab PDF. It opens an input file and reads each word and inserts it into
	// newly constructed RBT, AVL, BST, and Skip list datatypes.
	//
	char c;
	RBT* RBT_T = new RBT();			// instantiate each of the trees
	AVL* AVL_T = new AVL();			//
	BST* BST_T = new BST();			//
	SkipList* SL = new SkipList();	// and the skip list

	char chari[50]; // assumes no word is longer than 49 characters
	memset(chari, 0, 50); // zero the word buffer
	int iPtr;
	ifstream inFile;

	for (int pass = 0; pass < 6; pass++)
	{
		bool IsDelimiter = false, WasDelimiter = false;

		inFile.open("C:\\Users\\nicch\\OneDrive\\Documents\\College\\Spring 2020\\EECS 2510\\Lab 2\\Sample Files\\Shakespeare.txt", ios::binary);
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
			if (IsDelimiter && !WasDelimiter)   // if THIS character IS a delimiter,and the
												// last one WASN’T, it’s the end of a word
			{
				WasDelimiter = true;
				if (pass == 2) RBT_T->insert(chari);		// insert this word in the RBT
				else if (pass == 3) AVL_T->insert(chari);	// insert it in the AVL Tree
				else if (pass == 4) BST_T->insert(chari);	// insert it in the BST
				else if (pass == 5)    SL->insert(chari);	// insert it in the skip list

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
	// This is a very simple entrypoint to the program which just
	// calls the runTests method to start testing the datatypes.
	//
	runTests();

	return 0;
}