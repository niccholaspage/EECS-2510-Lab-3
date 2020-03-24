#pragma once

class AVL
{
public:
	AVL();
	~AVL();
private:
	struct node
	{
		char word[50] = {};
		int count = 1;
		node* leftChild = nullptr;
		node* rightChild = nullptr;
	};
};