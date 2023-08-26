/* Your code here! */
#include "dsets.h"
/*
    Adds unconnected root nodes to the end of our elements vector.
    Root nodes store the negative of the size of the disjoint set.
    An unconnected node has -1 as its value, because it is the only node in its set.
*/
void DisjointSets::addelements(int num){
    for (int i = 0; i < num; i++){
        nodes.push_back(-1);  // stores num -1's at the end of elements
    }
}

/*
    Finds the root of the given element. Elements that are representative nodes store negative values,
    otherwise, the value at each node points to its parent. As we traverse through the set, perform path compression.
*/
int DisjointSets::find(int elem){  
    std::vector<int> traversed; //stores the traversed indices for path compression
    int root = elem; //the representative node for the disjoint set
    
    while (elem >= 0) {  //traverse through set until we reach the root node
        traversed.push_back(elem);
        if (nodes[elem] < 0){ root = elem; }
        elem = nodes[elem];
    }
    for (int index : traversed) {  //for every traversed node that is not the root, have it point to the root
        if (index != root){
            nodes[index] = root;
        }
    }
    return root;
}

/*
    Creates the union of two sets, using union-by-size. 
    The smaller set points to the larger set. If the sets are equal size,
    the second tree points to the first. 
    Calls find on each element before union-ing the trees
*/
void DisjointSets::setunion(int a, int b){
    int repA = find(a);
    int repB = find(b);   // find the two representative nodes of the two given sets

    int newSize = nodes[repA] + nodes[repB];  //new size is the sum of the two old sizes
    if(nodes[repB] < nodes[repA]){
        nodes[repA] = newSize;
        nodes[repB] = repA;
    } else {
        nodes[repB] = newSize;
        nodes[repA] = repB;
    }
}

/*
    Returns the number of nodes in the up-tree containing the given element
*/
int DisjointSets::size(int elem){
    int root = find(elem);  // finds the representative node of the set, which stores the negative size
    return -1 * nodes[root];  //returns the positive size by multiplying by -1
}