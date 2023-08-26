/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */


template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    Node* temp = t->right;
    Node* tempLeft = temp->left;
    t->right = tempLeft;
    temp->left = t;
    t->height = std::max(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;
    temp->height = std::max(heightOrNeg1(temp->left), heightOrNeg1(temp->right)) + 1;
    t = temp;
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    Node* temp = t->left;
    Node* tempRight = temp->right;
    t->left = tempRight;
    temp->right = t;
    t->height = std::max(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;
    temp->height = std::max(heightOrNeg1(temp->left), heightOrNeg1(temp->right)) + 1;
    t = temp;
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
    if (subtree == NULL){
        return;
    }
    int balanceFactor = heightOrNeg1(subtree->right) - heightOrNeg1(subtree->left);
    if(balanceFactor < -1){
        int internalBalanceFactor = heightOrNeg1(subtree->left->right) - heightOrNeg1(subtree->left->left);
        if (internalBalanceFactor < 0){
            rotateRight(subtree);
        } else {
            rotateLeftRight(subtree);
        }
    } else if (balanceFactor > 1){
        int internalBalanceFactor = heightOrNeg1(subtree->right->right) - heightOrNeg1(subtree->right->left);
        if (internalBalanceFactor < 0){
            rotateRightLeft(subtree);
        } else {
            rotateLeft(subtree);
        }
    }
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    if(subtree == NULL){
        subtree = new Node(key, value);
    } else if (subtree->key < key){
        insert(subtree->right, key, value);
    } else if (subtree->key > key){
        insert(subtree->left, key, value);
    }
    subtree->height = std::max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right)) + 1;
    rebalance(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        // your code here
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        // your code here
        remove(subtree->right, key);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            // your code here
            delete subtree;
            subtree = NULL;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            // your code here
            Node * IOP = subtree->left;
            while (IOP->right != NULL){
                IOP = IOP->right;
            }
            swap(subtree, IOP);
            remove(subtree->left, key);
        } else {
            /* one-child remove */
            // your code here
            if(subtree->right == NULL){
                Node * temp = subtree;
                subtree = subtree->left;
                delete temp;
            } else {
                Node * temp = subtree;
                subtree = subtree->right;
                delete temp;
            }
        }
        // your code here
    }
    rebalance(subtree);
}
