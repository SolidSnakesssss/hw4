#include <iostream>
#include <map>
#include "bst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',7));
    bt.insert(std::make_pair('b',2));
		bt.insert(std::make_pair('c',4));
		bt.insert(std::make_pair('d',9));
		//bt.insert(std::make_pair('e',10));
		//bt.insert(std::make_pair('f',1));
		//bt.insert(std::make_pair('g',6));

		bt.print();
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }

		char target = 'g';

    if(bt.find(target) != bt.end()) {
        cout << "Found " << target << endl;
    }
    else {
        cout << "Did not find " <<  target << endl;
    }
    cout << "Erasing " << target << endl;
    bt.remove(target);

    return 0;
}