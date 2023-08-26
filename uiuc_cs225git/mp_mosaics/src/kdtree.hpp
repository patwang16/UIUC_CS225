/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>
#include <cmath>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim] == second[curDim]){
      return first<second;
    }
    return first[curDim] < second[curDim];
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
    double sqrdDistanceCurBest = 0.0;
    double sqrdDistancePotential = 0.0;
    for (int i = 0; i < Dim; i++){
      sqrdDistanceCurBest += pow(currentBest[i] - target[i], 2);
      sqrdDistancePotential += pow(potential[i] - target[i], 2);
    }
    if (sqrdDistanceCurBest == sqrdDistancePotential){
      return potential < currentBest;
    }
    return sqrdDistancePotential < sqrdDistanceCurBest;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    size = newPoints.size();
    if(newPoints.empty()){
      root = NULL;
    } else {
      auto temp = vector(newPoints);
      root = recursiveConstruct(temp, 0, temp.size() - 1, 0);
    }
    
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::recursiveConstruct(vector<Point<Dim>> & points, int start, int end, int curDim){
  if (start > end){
    return NULL;
  }
  int tempDim = Dim;
  int median = (start + end) / 2;
  auto smaller = [curDim] (auto first, auto second) {
    return smallerDimVal(first, second, curDim);
  };
  select(points.begin() + start, points.begin() + end + 1, points.begin() + median, smaller);
  KDTreeNode* ret = new KDTreeNode(points[median]);
  ret->left = recursiveConstruct(points, start, median - 1, (curDim + 1) % Dim);
  ret->right = recursiveConstruct(points, median + 1, end, (curDim + 1) % Dim);
  return ret;
}


template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  size = other.getSize();
  copy(root, other.getRoot());
}


template <int Dim>
void KDTree<Dim>::copy(KDTreeNode *& root, KDTreeNode * other){
  if (other == NULL){
    return;
  }
  root = new KDTreeNode(other->point);
  copy(root->left, other->left);
  copy(root->right, other->right);
}

template <int Dim>
void KDTree<Dim>::clear(KDTreeNode * root){
  if(root == NULL){
    return;
  }
  clear(root->left);
  clear(root->right);
  delete root;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */

  this->size = rhs.getSize();
  clear(root);
  copy(root, rhs.getRoot());

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  clear(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    

    return (recursiveFind(query, root, 0))->point;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::recursiveFind(const Point<Dim> & query, KDTreeNode* rt, int curDim) const
{
  if(rt == NULL){
    return NULL;
  }
  if(rt->left == NULL && rt->right == NULL){
    return rt;
  }

  KDTreeNode * currentBest = rt;

  if(smallerDimVal(query, rt->point, curDim)){
    if(rt->left != NULL){
      currentBest = recursiveFind(query, rt->left, (curDim + 1) % Dim);
    }
  } else {
    if(rt->right != NULL){
      currentBest = recursiveFind(query, rt->right, (curDim + 1) % Dim);
    }
  }

  if(shouldReplace(query, currentBest->point, rt->point)){
    currentBest = rt;
  }
  int currentRadius = 0;
  for (int i = 0; i < Dim; i++){
    currentRadius += pow(currentBest->point[i] - query[i], 2);
  }

  int splitting = pow(rt->point[curDim] - query[curDim], 2);

  KDTreeNode * checkOpposite = currentBest;

  if(currentRadius >= splitting){
    if(smallerDimVal(query, rt->point, curDim)){
      if(rt->right != NULL) {
        checkOpposite = recursiveFind(query, rt->right, (curDim + 1) % Dim);
      }
    } else{
      if(rt->left != NULL) {
        checkOpposite = recursiveFind(query, rt->left, (curDim + 1) % Dim);
      }
    }
    if(shouldReplace(query, currentBest->point, checkOpposite->point)){
      currentBest = checkOpposite;
    }
  }

  return currentBest;
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */    
    if (start == end || start == end - 1){
      return;
    }
    int length = 0;
    for (auto it = start; it != end; it++){
      length++;
    }
    RandIter pivot = start + length/2;
    pivot = partition(start, end, pivot, cmp);
    if (k == pivot){
      return;
    } else if (k < pivot) {
      select(start, pivot, k, cmp);
    } else {
      select(pivot + 1, end, k, cmp);
    }
    
}

template <typename RandIter, typename Comparator>
RandIter partition(RandIter start, RandIter end, RandIter pivot, Comparator cmp)
{
    //helper for select
  //std::cout<< *pivot << std::endl;
  RandIter end_ = end - 1;
  auto pivotValue = *pivot;
  *pivot = *end_;
  *end_ = pivotValue;
  RandIter storeIndex = start;
  for (auto it = start; it != end - 1; it++){
    //std::cout<< *it << std::endl;
    if (cmp(*it, pivotValue)){
      auto temp = *it;
      *it = *storeIndex;
      *storeIndex = temp;
      storeIndex++;
    }
  }
  
  //std::cout<< "check" << std::endl;
  auto tempf = *end_;
  *end_ = *storeIndex;
  *storeIndex = tempf;
  return storeIndex;
}