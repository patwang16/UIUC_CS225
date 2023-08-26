/**
 * @file binarytree.hpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
#include "InorderTraversal.h"
#include <iostream>

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + std::max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Call recursive helper function on the root
    printLeftToRight(root);

    // Finish the line
    std::cout << std::endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
    // Base case - null node
    if (subRoot == NULL)
        return;

    // Print left subtree
    printLeftToRight(subRoot->left);

    // Print this node
    std::cout << subRoot->elem << ' ';

    // Print right subtree
    printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
    template <typename T>
void BinaryTree<T>::mirror()
{
    //your code here
    mirror(root);
}

template <typename T>
void BinaryTree<T>::mirror(Node * subRoot) 
{
    if (subRoot == NULL){
        return;
    }
    Node * temp = subRoot->left;
    subRoot->left = subRoot->right;
    subRoot->right = temp;
    mirror(subRoot->left);
    mirror(subRoot->right);
}

/**
 * isOrdered() function iterative version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedIterative() const
{
    // your code here
    InorderTraversal<T> t(root);
    auto it = t.begin();
    while(it != t.end()){
        auto temp = (*it)->elem;
        ++it;
        //std::cout << *it << ' ' << temp << std::endl;
        if((*it) != NULL){
            if((*it)->elem < temp){
                return false;
            }
        }   
    }
    return true;
}

/**
 * isOrdered() function recursive version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedRecursive() const
{
    // your code here
    return isOrderedRecursive(root);
}

template <typename T>
bool BinaryTree<T>::isOrderedRecursive(const Node * subRoot) const
{
    // your code here
    if (subRoot == NULL){
        return true;
    } 
    if(subRoot->left == NULL){
        if(subRoot->right == NULL){
            return true;
        }
        if(isOrderedRecursive(subRoot->right)){
            if(smallest(subRoot->right) > subRoot->elem){
                return true;
            }
        }
    }
    else if(subRoot->right == NULL){
        if(isOrderedRecursive(subRoot->left)){
            T largestLeft = largest(subRoot->left);
            if(subRoot->elem > largestLeft){
                return true;
            }
        }
    }
    else if(isOrderedRecursive(subRoot->left)){
        T largestLeft = largest(subRoot->left);
        if(subRoot->elem > largestLeft){
            if(isOrderedRecursive(subRoot->right)){
                if(subRoot->elem < smallest(subRoot->right)){
                    return true;
                }
            }
        }
    }
    return false;
}

template <typename T>
T BinaryTree<T>::largest(const Node * subRoot) const
{
    if (subRoot == NULL){
        return T();
    }
    T temp = std::max(subRoot->elem, largest(subRoot->left));
    return std::max(temp, largest(subRoot->right));
}

template <typename T>
T BinaryTree<T>::smallest(const Node * subRoot) const
{
    if (subRoot == NULL){
        return T();
    }
    if (subRoot->right == NULL){
        if(subRoot->left == NULL){
            return subRoot->elem;
        }
        return std::min(subRoot->elem, smallest(subRoot->left));
    } else if (subRoot->left == NULL){
        return std::min(subRoot->elem, smallest(subRoot->right));
    } else {
        T temp = std::min(subRoot->elem, smallest(subRoot->left));
        return std::min(temp, smallest(subRoot->right));
    }   
}


