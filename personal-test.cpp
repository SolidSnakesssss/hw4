#include <iostream>
#include <map>
#include "bst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<int, double> bt;

		bt.insert(std::make_pair(2, 1.0));
		bt.insert(std::make_pair(1, 1.0));
		bt.insert(std::make_pair(3, 1.0));

		bt.print();

		bt.remove(2);
		
		/*
		bt.insert(std::make_pair(10, 0x5a));
		bt.insert(std::make_pair(15, 0xb));
		bt.insert(std::make_pair(2, 4));
		bt.insert(std::make_pair(4, 159));
		*/

		bt.print();
    
    //cout << "Binary Search Tree contents:" << endl;

		/*
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }

		char target = 'b';

    if(bt.find(target) != bt.end()) {
        cout << "Found " << target << endl;
    }
    else {
        cout << "Did not find " <<  target << endl;
    }
    cout << "Erasing " << target << endl;

		cout << "Club" << endl;
    bt.remove(target);

		cout << "Club Baseball" << endl;
		*/

    return 0;
}