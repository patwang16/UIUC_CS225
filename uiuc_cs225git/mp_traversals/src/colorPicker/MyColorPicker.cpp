#include "cs225/HSLAPixel.h"
#include "../Point.h"

#include "ColorPicker.h"
#include "MyColorPicker.h"
#include <stdlib.h>

using namespace cs225;

MyColorPicker::MyColorPicker() { srand((unsigned) time(NULL)); };

/**
 * Picks the color for pixel (x, y).
 * Using your own algorithm
 */
HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y) {
  /* @todo [Part 3] */
  return HSLAPixel(rand() % 360, 1, 0.75);
}
