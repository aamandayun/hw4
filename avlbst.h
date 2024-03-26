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
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* p, int diff);

    void rotateRight(AVLNode<Key, Value>* g);
    void rotateLeft(AVLNode<Key, Value>* p);

    bool isZigZigLeft(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p, AVLNode<Key, Value>* g);
    bool isZigZigRight(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p, AVLNode<Key, Value>* g);
    bool isZigZagLeft(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p, AVLNode<Key, Value>* g);
    bool isZigZagRight(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p, AVLNode<Key, Value>* g);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* n;
    AVLNode<Key, Value>* root = dynamic_cast<AVLNode<Key, Value>*>(this->root_);

    if(root==nullptr){
      root = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      root->setBalance(0);
      this->root_ = root;
      return;
    }

    AVLNode<Key, Value>* curr = root;
    
    //walk tree
    while(true){
      if(curr->getKey()==new_item.first){
        curr->setValue(new_item.second);
        //need to set the balance to something?
        return;
      }else if(curr->getKey() > new_item.first){
        if(curr->getLeft()==nullptr){
          curr->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, curr));
          curr->getLeft()->setBalance(0);
          n = curr->getLeft();
          //curr is the parent of new node
          if(curr->getBalance()==-1 || curr->getBalance()==1 ){
            curr->setBalance(0);
          }else if(curr->getBalance()==0){
            curr->updateBalance(-1);
            //curr->setBalance(-1);
            AVLNode<Key, Value>* p = curr;
            insertFix(p, n);
          }
          return;
        }else{
          curr=curr->getLeft();
        }
      }else{
        if(curr->getRight()==nullptr){
          curr->setRight(new AVLNode<Key,  Value>(new_item.first, new_item.second, curr));
          curr->getRight()->setBalance(0);
          n = curr->getRight();

          
          if(curr->getBalance()==-1 || curr->getBalance()==1 ){
            curr->setBalance(0);
          }else if(curr->getBalance()==0){
            curr->updateBalance(1);
            //curr->setBalance(1);
            AVLNode<Key, Value>* p = curr;
            insertFix(p, n);
          }
          return;
        }else{
          curr=curr->getRight();
        }
      }
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
void AVLTree<Key, Value>:: insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n)
{
    // TODO
    //if p is null then return
    if(p==nullptr){
      return;
    }
    int pBalance = p->getBalance();
    int nBalance = n->getBalance();

//our of balance.
//removed pBalance!=0 and nBalance !=0
    if(pBalance != -1 && pBalance != 1 && nBalance != -1 && nBalance != 1){
      return;
    }


//if parent p is null then return
  AVLNode<Key, Value>* g = p->getParent();
  if(g==nullptr){
    return;
  }

  bool zigzigLeft=isZigZigLeft(n, p, g);
  bool zigzigRight = isZigZigRight(n, p, g);
  bool zigzagLeft = isZigZagLeft(n, p, g);
  bool zigzagRight = isZigZagRight(n, p, g);

  if(zigzigRight || zigzagRight){
    g->updateBalance(1);
  }else if(zigzigLeft || zigzagLeft){
    g->updateBalance(-1);
  }
  
  if(g->getBalance()==0){
    return;
  }else if(g->getBalance()==-1 || g->getBalance() == 1){
    insertFix(g, p);
  }else if(g->getBalance() == -2 || g->getBalance()== 2){
    if(zigzigLeft==1){
      rotateRight(g);
      p->setBalance(0);
      g->setBalance(0);
    }else if(zigzigRight==1){
      rotateLeft(g);
      p->setBalance(0);
      g->setBalance(0);
    }else{ //ZIGZAG
      //need to rotate opposite for zigzagright???s
      if(zigzagLeft==1){
        rotateLeft(p);
        rotateRight(g);
      }else if(zigzagRight==1){
        rotateRight(p);
        rotateLeft(g);
      }
      if(n->getBalance()==-1 && zigzagLeft==1){
        p->setBalance(0);
        g->setBalance(1); 
        n->setBalance(0);
      }else if(n->getBalance()==1 && zigzagRight==1){
        p->setBalance(0);
        g->setBalance(-1); 
        n->setBalance(0);
      }
      else if(n->getBalance()==0){
        p->setBalance(0);
        g->setBalance(0);
        n->setBalance(0);
      }else if(n->getBalance()==1 && zigzagLeft==1){
        p->setBalance(-1);
        g->setBalance(0);
        n->setBalance(0);
      }else if(n->getBalance()== -1 && zigzagRight==1){
        p->setBalance(1);
        g->setBalance(0);
        n->setBalance(0);
      }
    }
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>:: rotateRight(AVLNode<Key, Value>* g)
{
    // TODO

    if(g==nullptr || g->getLeft()==nullptr){
        return;
    }
//taking a left child, making it the parent
  AVLNode<Key, Value>* originalParent = g->getParent(); 
  AVLNode<Key, Value>* newParent = g->getLeft();
  AVLNode<Key, Value>* newRight = newParent->getRight();

  newParent->setParent(originalParent);
  g->setLeft(newRight);

  if(newRight!=nullptr){
    newRight->setParent(g);
  }

  if(originalParent==nullptr){
    this->root_ = newParent;
  }else{
    if(originalParent->getLeft()==g){
      originalParent->setLeft(newParent);
    }else{
      originalParent->setRight(newParent);
    }
  }

  //swap grandparent and right child?
  // nodeSwap(g, newParent);

  newParent->setRight(g);
  g->setParent(newParent);


}


template<class Key, class Value>
void AVLTree<Key, Value>:: rotateLeft(AVLNode<Key, Value>* g)
{
  // TODO
  if(g==nullptr || g->getRight()==nullptr){
    return;
  }
//taking a right child, making it the parent
  AVLNode<Key, Value>* originalParent = g->getParent(); 
  AVLNode<Key, Value>* newParent = g->getRight();
  AVLNode<Key, Value>* newRight = newParent->getLeft();

  newParent->setParent(originalParent);
  g->setRight(newRight);

  if(newRight!=nullptr){
    newRight->setParent(g);
  }

  if(originalParent==nullptr){
    this->root_ = newParent;
  }else{
    if(originalParent->getLeft()==g){
      originalParent->setLeft(newParent);
    }else{
      originalParent->setRight(newParent);
    }
  }

  //swap grandparent and right child?
  // nodeSwap(g, newParent);

  newParent->setLeft(g);
  g->setParent(newParent);

    
}

template<class Key, class Value>
bool AVLTree<Key, Value>:: isZigZigLeft(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p, AVLNode<Key, Value>* g)
{
  // TODO
  if(g->getLeft()==p && p->getLeft() == n){
    return true;
  }

  return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>:: isZigZigRight(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p, AVLNode<Key, Value>* g)
{
  // TODO
  if(g->getRight()==p && p->getRight() == n){
    return true;
  }

  return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>:: isZigZagLeft(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p, AVLNode<Key, Value>* g)
{
  // TODO
  if(g->getLeft()==p && p->getRight() == n){
    return true;
  }

  return false;
}

template<class Key, class Value>
bool AVLTree<Key, Value>:: isZigZagRight(AVLNode<Key, Value>* n, AVLNode<Key, Value>* p, AVLNode<Key, Value>* g)
{
  // TODO
  if(g->getRight()==p && p->getLeft() == n){
    return true;
  }

  return false;
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if(this->root_==nullptr){
      return;
    }
    AVLNode<Key, Value>* root = dynamic_cast<AVLNode<Key, Value>*>(this->root_);


    AVLNode<Key, Value>* parent = nullptr;
    AVLNode<Key, Value>* curr = root;



    int diff = 0;

    //find node n to remove by walking the tree
    while(curr!=nullptr && curr->getKey()!=key){
        parent = curr;
        if(key<curr->getKey()){
            curr = curr->getLeft();
        }else{
            curr = curr->getRight();
        }
    }

    if(curr==nullptr){
        return; //couldn't find node to remove
    }

    //node is a leaf with no children
    if(curr->getLeft()==nullptr && curr->getRight() == nullptr){
        if(parent == nullptr){
            delete curr;
            this->root_ = nullptr;
        }else if(parent->getLeft()==curr){
            diff = 1; 
            delete parent->getLeft();
            parent->setLeft(nullptr);
        }else{
            diff = -1;
            delete parent->getRight();
            parent->setRight(nullptr);
        }
    }

    //one child
    else if(curr->getLeft()==nullptr || curr->getRight()==nullptr){
        AVLNode<Key, Value>* child;
        if(curr->getLeft()!=nullptr){
            child = curr->getLeft();
        }else{
            child = curr->getRight();
        }

        if(parent==nullptr){
            this->root_=child; //should be root = child?
            child->setParent(nullptr);
        }else if(parent->getLeft()==curr){
            parent->setLeft(child);
            child->setParent(parent);
            diff = 1;
        }else{
            parent->setRight(child);
            child->setParent(parent);
            diff = -1;
        }
        delete curr;

    }

    //two children
    //Node<Key, Value>* parent = curr->getParent();
    else{
        AVLNode<Key, Value>* pre;
        while(curr->getRight()!=nullptr || curr->getLeft()!=nullptr){
          pre = curr->getLeft();
          while(pre->getRight()!=nullptr){
            pre = pre->getRight();
          }
          nodeSwap(curr, pre);
        }
        parent = curr->getParent();
        if(parent==nullptr){
          delete curr;
          //need to set children's parent to null?
          return;
        }else if(parent->getRight()==curr){
          delete curr;
          parent->setRight(nullptr);
          diff = -1;
        }else{
          delete curr;
          parent->setLeft(nullptr);
          diff =1;
         }
    }
    
    //patch tree by calling removeFix(p, diff)

    removeFix(parent, diff);
}



template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key, Value>* n, int diff)
{
    //if n is null then return
    if(n==nullptr){
        return;
    }


    int ndiff = 0;

    //let p = parent(n) and if p is not NULL let ndiff(nextdiff) = +1 if n is a left child and -1 otherwise
    AVLNode<Key, Value>* p= n->getParent();
    if(p!=nullptr){
        if(p->getLeft() == n){
            ndiff = 1;
        }else{
            ndiff = -1;
        }
    }

    //case 1a
    if(n->getBalance() + diff == -2){
        AVLNode<Key, Value>* c = n->getLeft();
        if(c->getBalance()==-1){
            rotateRight(n);
            n->setBalance(0);
            c->setBalance(0);
            removeFix(p, ndiff);
        }else if(c->getBalance() == 0){
            rotateRight(n);
            n->setBalance(-1);
            c->setBalance(1);
            return;
        }else if(c->getBalance()== 1){
            AVLNode<Key, Value>* g = c->getRight();
            rotateLeft(c);
            rotateRight(n);
            if(g->getBalance()==1){
                n->setBalance(0);
                c->setBalance(-1);
                g->setBalance(0);
            }else if(g->getBalance()==0){
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }else if(g->getBalance()==-1){
                n->setBalance(1);
                c->setBalance(0);
                g->setBalance(0);
            }

            removeFix(p, ndiff);
        }
    }
//case 1b
    else if(n->getBalance() + diff == 2){
        AVLNode<Key, Value>* c = n->getRight(); //flipped
        if(c->getBalance()==1){ //changed
            rotateLeft(n); //flipped
            n->setBalance(0);
            c->setBalance(0);
            removeFix(p, ndiff);
        }else if(c->getBalance() == 0){
            rotateLeft(n); //flipped
            n->setBalance(1);//changed
            c->setBalance(-1);//changed
            return;
        }else if(c->getBalance()== -1){ //changed
            AVLNode<Key, Value>* g = c->getLeft(); //flipped
            rotateRight(c); //flipped
            rotateLeft(n); //flipped
            if(g->getBalance()==-1){ //changed
                n->setBalance(0);
                c->setBalance(1);  //changed
                g->setBalance(0);
            }else if(g->getBalance()==0){
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }else if(g->getBalance()==1){ //changed
                n->setBalance(-1); //changed
                c->setBalance(0);
                g->setBalance(0);
            }

            removeFix(p, ndiff);
        }
    }
    //case 2
    else if(n->getBalance() + diff == -1){
        n->setBalance(-1);
        return;
    }else if(n->getBalance()+ diff ==1){ //added this
      n->setBalance(1);
      return;
    }

    //case 3
    else if(n->getBalance() + diff ==0){
        n->setBalance(0);
        removeFix(p, ndiff);
    }

}





#endif
