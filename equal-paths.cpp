#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
// Helper function prototypes
int findHeight(Node* subtree);
bool equalLengthPaths(Node* node, int height, int maxHeight);

bool equalPaths(Node* root) {
	if(root == nullptr){
		return true;
	}
	
	int maxHeight = findHeight(root);

	return equalLengthPaths(root, 1, maxHeight);
}

int findHeight(Node* tree)
{
	if(tree == nullptr)
	{
		return 0;
	}

	int leftHeight = findHeight(tree->left);
	int rightHeight = findHeight(tree->right);

	if(leftHeight > rightHeight){
		return 1 + leftHeight;
	}

	else{
		return 1 + rightHeight;
	}
}

bool equalLengthPaths(Node* node, int height, int maxHeight)
{
	if (node == nullptr){
		return true;
	}

	if (node->right == nullptr && node->left == nullptr){
		return height == maxHeight;
	}

	return equalLengthPaths(node->left, height+1, maxHeight) 
	&& equalLengthPaths(node->right, height+1, maxHeight);
}