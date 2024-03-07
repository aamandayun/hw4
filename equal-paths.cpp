#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int getHeight(Node* node){
    if(node==nullptr){
      return 0;
    }

    int leftHeight=getHeight(node->left);
    int rightHeight = getHeight(node->right);

    if(leftHeight>rightHeight){
      return leftHeight+1;
    }

    return rightHeight+1;
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == nullptr){
      return true;
    }

    int left = getHeight(root->left);
    int right = getHeight(root->right);

    if((left==1 && right ==0) || (right ==1 && left ==0)){
      return true;
    }

    return left==right;

}

