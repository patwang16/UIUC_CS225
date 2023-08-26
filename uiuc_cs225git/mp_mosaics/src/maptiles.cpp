/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    std::vector<Point<3>> pixels;
    std::map<Point<3>, TileImage*> images;
    for (auto it = theTiles.begin(); it != theTiles.end(); it++){
        pixels.push_back(convertToXYZ((*it).getAverageColor()));
        images[convertToXYZ((*it).getAverageColor())] = &(*it);
    }
    MosaicCanvas* ret = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    KDTree<3> tree(pixels);
    for(int i = 0; i < ret->getRows(); i++){
        for (int j = 0; j < ret->getColumns(); j++){
            ret->setTile(i, j, images[tree.findNearestNeighbor(convertToXYZ(theSource.getRegionColor(i,j)))]);
        }
    }
    return ret;
}

