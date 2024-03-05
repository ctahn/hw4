#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


int twoChildren(Node* parent)
{
  if(parent->left == nullptr && parent->right == nullptr){
    return 1;
  }
  else if(parent->left == nullptr && parent->right != nullptr){
    return twoChildren(parent->right)+1;
  }
  else if(parent->left != nullptr && parent->right == nullptr){
    return twoChildren(parent->left)+1;
  }
  else{
    return twoChildren(parent->left)+1;
  }
}


bool equalPaths(Node * root)
{
  bool answer = true;
  if(root == nullptr){
    return answer;
  }
  if(root->left != nullptr){
    if(!answer){
      return false;
    }
    answer = equalPaths(root->left);
  }
  if(root->right != nullptr){
    if(!answer){
      return false;
    }
    answer = equalPaths(root->right);
  }
  if(root->left != nullptr && root->right != nullptr){
    if(!answer){
      return false;
    }
    int left = 0;
    int right = 0;
    left = twoChildren(root->left);
    right = twoChildren(root->right);
    if(left != right){
      answer = false;
    }
  }
  return answer;
}

