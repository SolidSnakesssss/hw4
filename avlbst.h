#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* p);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for p, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *p) :
    Node<Key, Value>(key, value, p), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the p since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		void rotateRight(AVLNode<Key, Value>* z);
		void rotateLeft(AVLNode<Key, Value>* x);
		void insertFix(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p);
		void removeFix(AVLNode<Key, Value>* n, int8_t diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
		//TODO
		//std::cout << new_item.first << ", " << std::endl;

		if(this->root_ == nullptr){
			this->root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);

			AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
			temp->setBalance(0);

			return;
		}

		else{
			AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
			AVLNode<Key, Value>* parent = nullptr;

			while(current != nullptr){
				parent = current;
				
				if(new_item.first < current->getKey()){
					current = current->getLeft();
				}

				else if(new_item.first > current->getKey()){
					current = current->getRight();
				}

				else{
					current->setValue(new_item.second);
					return;
				}
			}

			AVLNode<Key, Value>* newChild = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
			newChild->setBalance(0);

			if(new_item.first < parent->getKey()){
				parent->setLeft(newChild);
			}

			else{
				parent->setRight(newChild);
			}

			if(parent->getBalance() == -1 || parent->getBalance() == 1){
				parent->setBalance(0);
				return;
			}

			else{
				if(newChild == parent->getLeft()){
					parent->updateBalance(-1);
				}

				else{
					parent->updateBalance(1);
				}

				insertFix(parent, newChild);
			}
		}
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template <class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // Find node to remove (n) by walking the tree
    AVLNode<Key, Value>* n = static_cast<AVLNode<Key,Value>*>(this->root_);
    while (n != nullptr && key != n->getKey()) {
        if (key < n->getKey()) {
            n = n->getLeft();
        } else {
            n = n->getRight();
        }
    }

    // If node not found, return
    if (n == nullptr) {
        return;
    }

    AVLNode<Key, Value>* p = n->getParent(); // Parent node of n
    int8_t diff = 0; // Balance factor change

    // Handle case where n has only one child
    if (n->getLeft() != nullptr && n->getRight() != nullptr) {
        AVLNode<Key, Value>* succ = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::successor(n));
        nodeSwap(succ, n);
    }

    // Determine diff and adjust pointers if parent exists
    if (p != nullptr) {
        if (n == p->getLeft()) { // n is left child
            diff = 1;
            p->setLeft((n->getLeft() != nullptr) ? n->getLeft() : n->getRight());
        } else { // n is right child
            diff = -1;
            p->setRight((n->getLeft() != nullptr) ? n->getLeft() : n->getRight());
        }
        if (n->getLeft() != nullptr || n->getRight() != nullptr) { // Update child's parent pointer
            n->getLeft()->setParent(p);
            n->getRight()->setParent(p);
        }
        delete n; // Delete node n
    } else { // If n is the root
        if (n->getLeft() != nullptr || n->getRight() != nullptr) { // Update child's parent pointer
            if (n->getLeft() != nullptr) {
                this->root_ = n->getLeft();
            } else {
                this->root_ = n->getRight();
            }
            this->root_->setParent(nullptr);
        } else { // If n has no children
            this->root_ = nullptr;
        }
        delete n; // Delete node n
    }

    // Patch tree by calling removeFix
    if (p != nullptr) {
        removeFix(p, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* z) {
    if (z == nullptr) return; // Null pointer check

		bool isLeft = false;

		//Initialize pointers to the parent and child
		AVLNode<Key, Value>* y = z->getLeft();

		if(y == nullptr){
			return;
		}

		AVLNode<Key, Value>* c = y->getRight(); //Right node of y
		AVLNode<Key, Value>* g = z->getParent();

		if(g != nullptr){
			if(z == g->getLeft()){
				isLeft = true;
			}
		}

		//Rotation for y
		y->setRight(z);
		//y->setLeft(x);
		y->setParent(g);
		
		//Rotation for z
		z->setParent(y);
		z->setLeft(c);
		
		//Make z c's parent if c is not empty
		if(c != nullptr){
			c->setParent(z);
		}

		if(y->getParent() == nullptr){
			this->root_ = y;
		}

		else{
			if(isLeft) g->setLeft(y);
			else g->setRight(y);
		}
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x){
    if (x == nullptr) return; // Null pointer check

		//Initialize pointers to the parent and child
		AVLNode<Key, Value>* y = x->getRight();

		bool isLeft = false;

		if(y == nullptr){
			return;
		}

		AVLNode<Key, Value>* b = y->getLeft(); //Left node of y
		AVLNode<Key, Value>* g = x->getParent();

		if(g != nullptr){
			if(x == g->getLeft()){
				isLeft = true;
			}
		}

		//Rotation for y
		y->setLeft(x);
		y->setParent(g);

		//Rotation for x
		x->setParent(y);
		x->setRight(b);

		if(b != nullptr){
			b->setParent(x);
		}

		if(y->getParent() == nullptr){
			this->root_ = y;
		}

		else{
			if(isLeft) g->setLeft(y);
			else g->setRight(y);
		}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n){
	//n = child, p = parent, g = grandparent
	//Checks if parent is empty or is the root
	if (p == nullptr || p->getParent() == nullptr){
		
		return;
	}

	AVLNode<Key, Value>* g = p->getParent();

	//Checks if child is left node of parent
	if(p == g->getLeft()){
		g->updateBalance(-1);

		//Perfectly balanced as everything should be
		if(g->getBalance() == 0){
			return;
		}

		//Check if grandparent is out of balance
		else if(g->getBalance() == -1){
			insertFix(g, p);
		}

		//Le Parent out of balance
		else if(g->getBalance() == -2){
			//Zig-zig
			if(n == p->getLeft()){
				rotateRight(g);

				p->setBalance(0);
				g->setBalance(0);
			}

			//Zig-zag Left
			else {
				rotateLeft(p);

				rotateRight(g);
				//Case 3a
				if(n->getBalance() == -1){
					p->setBalance(0);
					g->setBalance(1);
					n->setBalance(0);
				}

				else if(n->getBalance() == 0){
					p->setBalance(0);
					g->setBalance(0);
					n->setBalance(0);
				}

				else if(n->getBalance() == 1){
					p->setBalance(-1);
					g->setBalance(0);
					n->setBalance(0);
				}
			}
		}
	}
	//End of if child is left node

	//Child is right node of parent
	else{
		g->updateBalance(1);

		//Perfectly balanced as everything should be
		if(g->getBalance() == 0){
			return;
		}

		//Check if grandparent is out of balance
		else if(g->getBalance() == 1){
			insertFix(g, p);
		}

		//Le Parent out of balance
		else if(g->getBalance() == 2){
			//Zig-zig
			if(n == p->getRight()){
				rotateLeft(g);

				p->setBalance(0);
				g->setBalance(0);
			}

			//Zig-zag
			else {
				rotateRight(p);
				rotateLeft(g);

				//Case 3a
				if(n->getBalance() == -1){
					p->setBalance(0);
					g->setBalance(1);
					n->setBalance(0);
				}

				else if(n->getBalance() == 0){
					p->setBalance(0);
					g->setBalance(0);
					n->setBalance(0);
				}

				else if(n->getBalance() == 1){
					p->setBalance(-1);
					g->setBalance(0);
					n->setBalance(0);
				}
			}
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff) {
    if (n == nullptr) {
        return;
    }
 
    AVLNode<Key, Value>* p = n->getParent();
    int8_t ndiff = 0;

    if (p != nullptr) {
        if (n == p->getLeft()) {
          ndiff = +1;
        } 
				
				else {
          ndiff = -1;
        }
    }

    if (diff == -1) {
        if (n->getBalance() + diff == -2) {
            AVLNode<Key, Value>* c = n->getLeft();

            if (c->getBalance() == -1) {
                rotateRight(n);
								//std::cout << "Candy" << std::endl;
                n->setBalance(0);
                c->setBalance(0);
								//std::cout << "Pog" << std::endl;
                removeFix(p, ndiff);
								return;
            } else if (c->getBalance() == 0) {
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
								return;
            } else {
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);

                if (g->getBalance() == 1) {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                } else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                } else {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                removeFix(p, ndiff);
								return;
            }
        } else if (n->getBalance() + diff == -1) {
            n->setBalance(-1);
            return;
        } else {
            n->setBalance(0);
            removeFix(p, ndiff);
						return;
        }
    } else if (diff == 1) {
        if (n->getBalance() + diff == 2) {
            AVLNode<Key, Value>* c = n->getRight();

            if (c->getBalance() == 1) {
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p, ndiff);
								return;
            } else if (c->getBalance() == 0) {
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
								return;
            } else {
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);

                if (g->getBalance() == -1) {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                } else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                } else {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }

                removeFix(p, ndiff);
								return;
            }
        } else if (n->getBalance() + diff == 1) {
            n->setBalance(1);
            return;
        } else {
            n->setBalance(0);
            removeFix(p, ndiff);
						return;
        }
    }
}

#endif