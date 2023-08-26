#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt( (h*h) + (s*s) + (l*l) );
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_back(point);
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_front(point);
  }

  /**
  * Removes and returns the current Point in the bfs traversal
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  */
  Point bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    auto ret = work_list[0];
    work_list.pop_front();
    return ret;
  }

  /**
  * Removes and returns the current Point in the dfs traversal
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  */
  Point dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    auto ret = work_list[0];
    work_list.pop_front();
    return ret;
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param traversal_add a function pointer to an implemented function which takes in a reference to the work list
  * and the point to be added
  * @param traversal_pop a function pointer to an implemented function which takes in a reference to the work list
  * and returns the next point to be processed in the traversal
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, add_function traversal_add, pop_function traversal_pop) {  
    /** @todo [Part 1] */
    iter = Iterator(png, start, tolerance, traversal_add, traversal_pop);
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    return iter;
  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return Iterator();
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
    work_list.resize(0);
  }

   ImageTraversal::Iterator::Iterator(const PNG & png, const Point & start, double tolerance, add_function traversal_add, pop_function traversal_pop) {
    /** @todo [Part 1] */
    png_ = png;
    start_ = Point(start.x, start.y);
    cur = Point(start.x, start.y);
    work_list.push_front(start);
    tolerance_ = tolerance;
    traversal_add_ = traversal_add;
    traversal_pop_ = traversal_pop;
    visited_.resize(png.width() * png.height());
  }

  void ImageTraversal::Iterator::setPoint(Point newPoint){
    cur = Point(newPoint.x, newPoint.y);
  }
  /**
  * Iterator increment opreator.
  *
  * Advances the traversal of the image.
  */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
    /** @todo [Part 1] */
    visited_[png_.width() * cur.y + cur.x] = true;
    HSLAPixel startPixel = png_.getPixel(start_.x, start_.y);
    
    std::vector<Point> neighbors {Point(cur.x + 1, cur.y), Point(cur.x, cur.y + 1), Point(int(cur.x) - 1, cur.y), Point(cur.x, int(cur.y - 1))};
    for (unsigned int i = 0; i < neighbors.size(); i++){
      if (neighbors[i].x >= 0 && neighbors[i].x < png_.width()){
        if (neighbors[i].y >= 0 && neighbors[i].y < png_.height()){
          HSLAPixel tempPixel = png_.getPixel(neighbors[i].x, neighbors[i].y);
          if (calculateDelta(startPixel, tempPixel) <= tolerance_){
            if(visited_[png_.width() * neighbors[i].y + neighbors[i].x] != true){
              traversal_add_(work_list, neighbors[i]);
            }
          }
        }
      }
    }
    neighbors.clear();

    while (!work_list.empty() && visited_[png_.width() * work_list[0].y + work_list[0].x] == true){
      traversal_pop_(work_list);
    }

    if (!work_list.empty()){
      cur = (work_list[0]);
    } else {
      cur = Point(-1,-1);
    }

    return *this;
  }

  /**
  * Iterator accessor opreator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    return cur;
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
    return !(empty() && other.empty());
  }

  bool ImageTraversal::Iterator::empty() const{
    return work_list.empty();
  }
}
