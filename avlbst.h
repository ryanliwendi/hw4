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
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
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
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
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
* An overridden function for getting the parent since a static_cast is necessary to make sure
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
    void insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void rotateLeft(AVLNode<Key,Value>* p);
    void rotateRight(AVLNode<Key,Value>* p);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this -> root_ == nullptr)
    {
        AVLNode<Key, Value> *node = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        this -> root_ = node;
        return;
    }
    AVLNode<Key, Value> *current = this -> root_;
    while (true)
    {
        if (new_item.first == current->getKey())
        {
            current -> setValue(new_item.second);
            return;
        }
        else if (new_item.first < current -> getKey())
        {
            if (current -> getLeft() == nullptr)
            {
                AVLNode<Key, Value> *node = new AVLNode<Key, Value>(new_item.first, new_item.second, current);
                current -> setLeft(node);
                break;
            }
            current = current->getLeft();
        }
        else if (new_item.first > current -> getKey())
        {
            if (current -> getRight() == nullptr)
            {
                AVLNode<Key, Value> *node = new AVLNode<Key, Value>(new_item.first, new_item.second, current);
                current -> setRight(node);
                break;
            }
            current = current->getRight();
        }
    }
    if (current -> getBalance() == -1 || current -> getBalance() == 1)
    {
        current -> setBalance(0);
        return;
    }
    if (current -> getLeft() != nullptr)
    {
        current -> setBalance(-1);
        insert_fix(current, current -> getLeft());
    }
    else
    {
        current -> setBalance(1);
        insert_fix(current, current -> getRight());
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    if (p == nullptr || n == nullptr)
        return;
    AVLNode<Key, Value> *g = p -> getParent();
    if (g == nullptr)
        return;
    if (g -> getLeft() == p)
    {
        g -> updateBalance(-1);
        if (g -> getBalance() == 0)
            return;
        else if (g -> getBalance() == -1)
            insert_fix(g, p);
        else if (g -> getBalance() == -2)
        {
            if (p -> getLeft() == n)
            {
                rotateRight(g);
                p -> setBalance(0);
                g -> setBalance(0);
            }
            else if (p -> getRight() == n)
            {
                rotateLeft(p);
                rotateRight(g);
                if (n -> getBalance() == -1)
                {
                    p -> setBalance(0);
                    g -> setBalance(1);
                    n -> setBalance(0);
                }
                else if (n -> getBalance() == 0)
                {
                    p -> setBalance(0);
                    g -> setBalance(0);
                    n -> setBalance(0);
                }
                else if (n -> getBalance() == 1)
                {
                    p -> setBalance(-1);
                    g -> setBalance(0);
                    n -> setBalance(0);
                }
            }
        }
    }
    else if (g -> getRight() == p)
    {
        g -> updateBalance(1);
        if (g -> getBalance() == 0)
            return;
        else if (g -> getBalance() == 1)
            insert_fix(g, p);
        else if (g -> getBalance() == 2)
        {
            if (p -> getRight() == n)
            {
                rotateLeft(g);
                p -> setBalance(0);
                g -> setBalance(0);
            }
            else if (p -> getLeft() == n)
            {
                rotateRight(p);
                rotateLeft(g);
                if (n -> getBalance() == 1)
                {
                    p -> setBalance(0);
                    g -> setBalance(-1);
                    n -> setBalance(0);
                }
                else if (n -> getBalance() == 0)
                {
                    p -> setBalance(0);
                    g -> setBalance(0);
                    n -> setBalance(0);
                }
                else if (n -> getBalance() == -1)
                {
                    p -> setBalance(1);
                    g -> setBalance(0);
                    n -> setBalance(0);
                }
            }
        }
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* p)
{
    if (p == nullptr || p -> getRight() == nullptr)
        return;
    AVLNode<Key, Value> *n = p -> getRight();
    AVLNode<Key, Value> *nl = n -> getLeft();
    n -> setParent(p -> getParent());
    if (p -> getParent() != nullptr)
    {
        if (p -> getParent() -> getLeft() == p)
            p -> getParent() -> setLeft(n);
        else
            p -> getParent() -> setRight(n);
    }
    else
        this -> root_ = n;

    n -> setLeft(p);
    p -> setRight(nl);
    p -> setParent(n);
    if (nl != nullptr)
        nl -> setParent(p);
}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* p)
{
    if (p == nullptr || p -> getLeft() == nullptr)
        return;
    AVLNode<Key, Value> *n = p -> getLeft();
    AVLNode<Key, Value> *nr = n -> getRight();
    n -> setParent(p -> getParent());
    if (p -> getParent() != nullptr)
    {
        if (p -> getParent() -> getLeft() == p)
            p -> getParent() -> setLeft(n);
        else
            p -> getParent() -> setRight(n);
    }
    else
        this -> root_ = n;

    n -> setRight(p);
    p -> setLeft(nr);
    p -> setParent(n);
    if (nr != nullptr)
        nr -> setParent(p);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
