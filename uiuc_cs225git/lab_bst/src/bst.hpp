/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    // your code here
    Node* nodeTemp = find(root, key);
    if (nodeTemp != NULL){
        return nodeTemp->value;
    } 
    return V();
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    // Your code here
    if (subtree == NULL){
        return subtree;
    }
    if (subtree->key == key){
        return subtree;
    }
    if (find(subtree->left, key) != NULL){
        return find(subtree->left, key);
    }
    return find(subtree->right, key);
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    // your code here
    insert(root, key, value);
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    if (find(subtree, key) != NULL){
        return;
    }
    if(subtree == NULL){
        subtree = new Node(key, value);
    } else if (subtree->key < key){
        insert(subtree->right, key, value);
    } else{
        insert(subtree->left, key, value);
    }
    
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    // your code here
    K firstKey = first->key;
    V firstValue = first->value;
    first->key = second->key;
    first->value = second->value;
    second->key = firstKey;
    second->value = firstValue;
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    // your code here
    remove(root, key);
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    // your code here
    if (subtree == NULL){
        return;
    }
    Node*& cur = find(subtree, key);
    if(cur == NULL){
        return;
    }
    if(cur->left == NULL){
        if(cur->right == NULL){
            delete cur;
            cur = NULL;
        }
        else{
            Node * temp = cur;
            cur = cur->right;
            delete temp;
        }
        return;
    }
    if(cur->right == NULL){
        Node * temp = cur;
        cur = cur->left;
        delete temp;
    } else{
        // find IOP
        Node * IOP = cur->left;
        while (IOP->right != NULL){
            IOP = IOP->right;
        }
        swap(cur, find(subtree,IOP->key));
        remove(cur->left, key);
    }
    return;
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    BST<K,V> ret;
    for (unsigned int i = 0; i < inList.size(); i++){
        ret.insert(inList[i].first, inList[i].second);
    }
    return ret;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    std::vector<int> ret;
    for (unsigned int i = 0; i < inList.size(); i++){
        ret.push_back(0);
    }
    std::sort(inList.begin(), inList.end());
    do{
        int h = listBuild(inList).height();
        ret[h] += 1;
    }while (std::next_permutation(inList.begin(), inList.end()));
    return ret;
}