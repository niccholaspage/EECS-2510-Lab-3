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
// Date:       Apr 4, 2020
// Copyright:  Copyright 2020 by Nicholas Nassar. All rights reserved.

#include <iostream>
#include <fstream>

#include "AVL.h"
#include "BST.h"
#include "RBT.h"
#include "SkipList.h"

using namespace std;

void runTests(char* file_path)
{
	// This method utilizes the file parser code given by Dr. Thomas in listing 2
	// of the lab PDF. It opens an input file at the given file path and reads
	// each word and inserts it into newly constructed RBT, AVL, BST, and Skip
	// list datatypes, than prints out statistics about each datatype.
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

	clock_t dryRunElapsedTime;

	cout << "File: " << file_path << "\n";

	for (int pass = 0; pass < 6; pass++)
	{
		// The time at the very beginning of our pass of the file
		clock_t startTime = clock();

		bool IsDelimiter = false, WasDelimiter = false;

		inFile.open(file_path, ios::binary);
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
												// last one WASN�T, it�s the end of a word
			{
				WasDelimiter = true;
				if (pass == 2) RBT_T->insert(chari);		// insert this word in the RBT
				else if (pass == 3) AVL_T->insert(chari);	// insert it in the AVL Tree
				else if (pass == 4) BST_T->insert(chari);	// insert it in the BST
				else if (pass == 5)    SL->insert(chari);	// insert it in the skip list

				memset(chari, 0, 50); // zero the word buffer
				iPtr = 0;
			}
			else if (!IsDelimiter)  chari[iPtr++] = c;  // if this isn�t a delimiter, keep going

			WasDelimiter = IsDelimiter;   // forthe NEXT iteration
			inFile.get(c);
		}
		inFile.close();
		// If the file doesn't end with a delimiter, it will leave the last word unprocessed.
		// Calculate the elapsed time, insert once more, and display the statistics for this structure...
		//
		clock_t endTime = clock();

		// We get the current time, subtract the start time from it, and divide by 1000
		// to get the elapsed time in seconds. We calculate the elapsed time here since
		// we don't want our printing and calculating of stats to be included in the elapsed
		// time.
		clock_t elapsedTime = endTime - startTime;

		if (pass == 2) { if (strlen(chari)) RBT_T->insert(chari); RBT_T->displayStatistics(); } // RBT
		else if (pass == 3) { if (strlen(chari)) AVL_T->insert(chari);  AVL_T->displayStatistics(); } // AVL
		else if (pass == 4) { if (strlen(chari)) BST_T->insert(chari);  BST_T->displayStatistics(); } // BST
		else if (pass == 5) { if (strlen(chari))    SL->insert(chari);     SL->displayStatistics(); } // skip list

		if (pass == 1)
		{
			dryRunElapsedTime = elapsedTime;
		}
		else if (pass > 1)
		{
			cout << "Elapsed Time: " << (elapsedTime - dryRunElapsedTime) / 1000.0 << " seconds\n"; // Print out the elapsed time we calculated above.
		}

	}

	delete RBT_T;
	delete AVL_T;
	delete BST_T;
	delete SL;
}

int main(int argc, char* argv[])
{
	// This is a very simple entrypoint to the program which just
	// takes the second argument as a file path and calls the
	// runTests method to start testing the datatypes.
	//
	if (argc < 2)
	{
		cout << "No file path provided.";

		return 0;
	}

	runTests(argv[1]);

	return 0;
}