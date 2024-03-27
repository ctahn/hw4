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
    AVLTree();
    virtual ~AVLTree();
    virtual void insert(const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    // Add helper functions here
    void insert_fix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node);
    void rotateRight(AVLNode<Key,Value>* node);
    void rotateLeft(AVLNode<Key,Value>* node);
    void removeFix(AVLNode<Key,Value>* node, int8_t diff);

    static AVLNode<Key, Value>* predecessor(AVLNode<Key, Value>* current);
};

template<class Key, class Value>
AVLTree<Key,Value>::AVLTree() : 
  BinarySearchTree<Key,Value>()
{
  
}

template<class Key, class Value>
AVLTree<Key,Value>::~AVLTree()
{
  BinarySearchTree<Key,Value>::clear();
}


template<class Key, class Value>
void AVLTree<Key,Value>::rotateRight(AVLNode<Key,Value>* node)
{
  if(node->getLeft() == NULL){
    return;
  }
  AVLNode<Key,Value>* temp = node->getLeft();
  
  node->setLeft(temp->getRight());
  if(node->getLeft() != NULL){
    node->getLeft()->setParent(node);
  }
  temp->setRight(node);
  temp->setParent(node->getParent());
  if(temp->getParent() == NULL){
    this->root_ = temp;
  }
  node->setParent(temp);  
  if(temp->getParent() != NULL){
    if(temp->getParent()->getKey() > temp->getKey()){
      temp->getParent()->setLeft(temp);
    }
    else{
      temp->getParent()->setRight(temp);
    }
  }
}


template<class Key, class Value>
void AVLTree<Key,Value>::rotateLeft(AVLNode<Key,Value>* node)
{
  if(node->getRight() == NULL){
    return;
  }
  AVLNode<Key,Value>* temp = node->getRight();  
  node->setRight(temp->getLeft());
  if(node->getRight()!= NULL){
    node->getRight()->setParent(node);
  }
  temp->setLeft(node);
  temp->setParent(node->getParent());
  if(temp->getParent() == NULL){
    this->root_ = temp;
  }
  node->setParent(temp); 
  
  if(temp->getParent() != NULL){
    if(temp->getParent()->getKey() > temp->getKey()){
      temp->getParent()->setLeft(temp);
    }
    else{
      temp->getParent()->setRight(temp);
    }
  }
}


template <class Key, class Value>
void AVLTree<Key,Value>::insert_fix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node)
{
  if(parent == NULL){
    return;
  }
  if(parent->getParent() == NULL){
    return;
  }
  if(parent->getParent()->getLeft() == parent){
    parent->getParent()->updateBalance(-1);
    if(parent->getParent()->getBalance() == 0){
    return;
    }
    else if(parent->getParent()->getBalance() == -1){
      return insert_fix(parent->getParent(), parent);
    }
    else{
      if(node == parent->getLeft()){
        rotateRight(parent->getParent());
        parent->setBalance(0);
        parent->getRight()->setBalance(0);
      }
      else{
        rotateLeft(parent);
        rotateRight(node->getParent());
        if(node->getBalance() == -1){
          parent->setBalance(0);
          node->getRight()->setBalance(1);
          node->setBalance(0);
        }
        else if(node->getBalance() == 0){
          parent->setBalance(0);
          node->getRight()->setBalance(0);
          node->setBalance(0);
        }
        else{
          parent->setBalance(-1);
          node->getRight()->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  }
  else if(parent->getParent()->getRight() == parent){
    parent->getParent()->updateBalance(1);
    if(parent->getParent()->getBalance() == 0){
      return;
    }
    else if(parent->getParent()->getBalance() == 1){
      return insert_fix(parent->getParent(), parent);
    }
    else{
      if(node == parent->getRight()){
        rotateLeft(parent->getParent());
        parent->setBalance(0);
        parent->getLeft()->setBalance(0);
      }
      else{
        rotateRight(parent);
        rotateLeft(node->getParent());
        if(node->getBalance() == 1){
          parent->setBalance(0);
          node->getLeft()->setBalance(-1);
          node->setBalance(0);
        }
        else if(node->getBalance() == 0){
          parent->setBalance(0);
          node->getLeft()->setBalance(0);
          node->setBalance(0);
        }
        else{
          parent->setBalance(1);
          node->getLeft()->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  }
}



/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    if(static_cast<AVLNode<Key,Value>*>(this->root_) == NULL){
      this->root_ = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
      return;
    }
    AVLNode<Key,Value>* node = static_cast<AVLNode<Key,Value>*>(this->root_);
    AVLNode<Key,Value>* parent = NULL;
    while(node != NULL){
      if(new_item.first < node->getKey()){
        parent = node;
        node = node->getLeft(); 

      }
      else if(new_item.first > node->getKey()){
        parent = node;
        node = node->getRight(); 

      }
      else{
        node->setValue(new_item.second);
        return;
      }
      if(node != NULL){
      }
    }
    node = new AVLNode<Key,Value>(new_item.first, new_item.second, parent);
    if(node->getKey() < node->getParent()->getKey()){
      node->getParent()->setLeft(node);
    }
    else{
      node->getParent()->setRight(node);
    }

    if(parent->getBalance() == -1){
      parent->setBalance(0);
    }
    else if(parent->getBalance() == 1){
      parent->setBalance(0);
    }
    else{
      if(parent->getKey() > node->getKey()){
        parent->updateBalance(-1);
        insert_fix(parent, node);
      }
      else{
        parent->updateBalance(1);
        insert_fix(parent, node);
      }
    }
}



template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* node, int8_t diff)
{
  if(node == NULL){
    return;
  }

  int8_t nextDiff = 0;
  if(node->getParent() != NULL){
    if(node->getParent()->getLeft() == node){
      nextDiff = 1;
    }
    else{
      nextDiff = -1;
    }
  }
  if(diff == -1){
    if(node->getBalance() + diff == -2){
      
      AVLNode<Key,Value>* tallerChild = node->getLeft();
      if(tallerChild->getBalance() == -1){
        rotateRight(node);
        node->setBalance(0);
        tallerChild->setBalance(0);
        removeFix(tallerChild->getParent(), nextDiff);
        return;
        
      }
      else if(tallerChild->getBalance() == 0){
        rotateRight(node);
        node->setBalance(-1);
        tallerChild->setBalance(1);
        return;
      }
      else{
        AVLNode<Key,Value>* right = tallerChild->getRight();
        rotateLeft(tallerChild);
        rotateRight(node);
        if(right->getBalance() == 1){
          node->setBalance(0);
          tallerChild->setBalance(-1);
          right->setBalance(0);
        }
        else if(right->getBalance() == 0){
          node->setBalance(0);
          tallerChild->setBalance(0);
          right->setBalance(0);
        }
        else{
          node->setBalance(1);
          tallerChild->setBalance(0);
          right->setBalance(0);
        }
        return removeFix(right->getParent(), nextDiff);
      }
    }
    else if(node->getBalance() + diff == -1){
      node->setBalance(-1);
      return;
    }
    else{
      node->setBalance(0);
      return removeFix(node->getParent(), nextDiff);
    }
  }
  else{
    if(node->getBalance() + diff == 2){
      AVLNode<Key,Value>* tallerChild = node->getRight();
      if(tallerChild->getBalance() == 1){
        rotateLeft(node);
        node->setBalance(0);
        tallerChild->setBalance(0);
        removeFix(tallerChild->getParent(), nextDiff);
        return;
      }
      else if(tallerChild->getBalance() == 0){
        rotateLeft(node);
        node->setBalance(1);
        tallerChild->setBalance(-1);
        return;
      }
      else{
        AVLNode<Key,Value>* left = tallerChild->getLeft();
        rotateRight(tallerChild);
        rotateLeft(node);
        if(left->getBalance() == -1){
          node->setBalance(0);
          tallerChild->setBalance(1);
          left->setBalance(0);
        }
        else if(left->getBalance() == 0){
          node->setBalance(0);
          tallerChild->setBalance(0);
          left->setBalance(0);
        }
        else{
          node->setBalance(-1);
          tallerChild->setBalance(0);
          left->setBalance(0);
        }
        return removeFix(left->getParent(), nextDiff);
      }
    }
    else if(node->getBalance() + diff == 1){
      node->setBalance(1);
      return;
    }
    else{
      node->setBalance(0);
      return removeFix(node->getParent(), nextDiff);
    }
  }
}



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  AVLNode<Key, Value>* node = static_cast<AVLNode<Key,Value>*>(this->root_);
  if(static_cast<AVLNode<Key,Value>*>(this->root_) == NULL){
    return;
  }

  while(node != NULL){
    if(key < node->getKey()){
      node = node->getLeft();
    }
    else if(key > node->getKey()){
      node = node->getRight();
    }
    else{
      break;
    }
    if(node == NULL){
      return;
    }
  }
  if(node->getRight() != NULL && node->getLeft() != NULL){
      AVLNode<Key,Value>* predecessor = this->predecessor(node);
      nodeSwap(predecessor, node);
      if(predecessor->getParent() == NULL){
        this->root_ = predecessor;
      }
  }
  int8_t diff = 0;
  AVLNode<Key,Value>* parent = node->getParent();
  if(parent != NULL){
    if(parent->getLeft() == node){
      diff = 1;
    }
    else{
      diff = -1;
    }
  }
  if(node->getRight() == NULL && node->getLeft() == NULL){
    
    if(node->getParent() == NULL){
      this->root_ = NULL;
    }
    else{
      if(node->getParent()->getLeft() == node){
        node->getParent()->setLeft(NULL);
      }
      else{
        node->getParent()->setRight(NULL);
      }
    }
  }
  else if(node->getRight() == NULL && node->getLeft() != NULL){
    if(node->getParent() == NULL){
      node->getLeft()->setParent(NULL);
      this->root_ = node->getLeft();
    }
    else{
      if(node->getParent()->getLeft() == node){
        node->getParent()->setLeft(node->getLeft());
        node->getLeft()->setParent(node->getParent());
      }
      else{
        node->getParent()->setRight(node->getLeft());
        node->getLeft()->setParent(node->getParent());
      }
    }
  }
  else{
    if(node->getParent() == NULL){
      node->getRight()->setParent(NULL);
      this->root_ = node->getRight();
    }
    else{
      if(node->getParent()->getLeft() == node){
        node->getParent()->setLeft(node->getRight());
        node->getRight()->setParent(node->getParent());
      }
      else{
        node->getParent()->setRight(node->getRight());
        node->getRight()->setParent(node->getParent());
      }
    }
  }
  delete node;
  removeFix(parent, diff);
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
AVLNode<Key,Value>* AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* node)
{
  AVLNode<Key, Value>* current = node;
  if(current == NULL){
    return NULL;
  }
  if(current->getLeft() != NULL){
    current = current->getLeft();
      while(true){
        if(current->getRight() != NULL){
          current = current->getRight();
        }
        else{
          return current;
        }
      }
    }
  else{
    if(current->getParent() == NULL){
      return NULL;
    }
    while(true){
      if(current->getParent()->getRight() == current){
        return current->getParent();
      }
      else{
        current = current->getParent();
      }
      if(current->getParent() == NULL){
        return NULL;
      }
    }
  }
}
#endif
