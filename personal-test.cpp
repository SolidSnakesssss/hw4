#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
		AVLTree<uint16_t, uint16_t> testTree;

		testTree.insert(std::make_pair(2, 9));
		testTree.print();

		testTree.insert(std::make_pair(0, 8));
		testTree.print();

		testTree.insert(std::make_pair(1, 159));
		testTree.print();

    return 0;
}