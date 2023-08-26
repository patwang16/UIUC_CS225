/* Your code here! */
#include "maze.h"

//Creates a completley empty maze
SquareMaze::SquareMaze(){
    _width = 0;
    _height = 0;
    _walls = new std::vector<wall>;
    _dset = new DisjointSets();
}

//Deconstructs maze by calling private _destroy() function
SquareMaze::~SquareMaze(){
    _destroy();
}

//Makes a new SquareMaze with given width & height. Clears all existing data, and then it randomly selects walls to delete. 
//Disjoint sets are used to prevent cycles from being created within the maze
void SquareMaze::makeMaze(int width, int height){
    //set new parameters for maze
    _width = width;
    _height = height;
    
    //clear existing data
    delete _walls;
    delete _dset;
    _walls = new std::vector<wall>;
    _dset = new DisjointSets();

    //add 1 new element for each square in the maze
    _dset->addelements(_width * _height);
    std::vector<int> unchecked;

    for (int i = 0; i < _width * _height; i++){
        _walls->push_back(wall(true, true));  //the maze starts with having all possible walls
        unchecked.push_back(i); //the numbers represent places in the maze using raster order
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(unchecked.begin(), unchecked.end(), g);

    while (!unchecked.empty()){
        int curr = unchecked[unchecked.size() - 1];
        unchecked.pop_back();

        int x = curr % _width;
        int y = curr / _width;

        //so long as the right wall is not an edge
        //remove the right wall
        if(((curr + 1) % _width != 0) && (_dset->find(curr) != _dset->find(curr + 1))){
            _dset->setunion(curr, curr + 1);
            setWall(x, y, 0, false);
        }

        //so long as the bottom wall is not an edge
        //remove the bottom wall
        if(((curr + _width) < _width * _height) && (_dset->find(curr) != _dset->find(curr + _width))){
            _dset->setunion(curr, curr + _width);
            setWall(x, y, 1, false);
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const{
    switch (dir) { //handle each direction separately, switch statement used as opposed to a bunch of if-else
        //right
        case 0:
            //check bounds
            if(x < _width - 1) 
                return !(*_walls)[x + y * _width]._right;
            break;

        //down
        case 1:
            //check bounds
            if(y < _height - 1) 
                return !(*_walls)[x + y * _width]._below;
            break;

        //left
        case 2:
            //check bounds
            if(x > 0 && x < _width) 
                return !(*_walls)[x + y * _width - 1]._right;
            break;

        //up
        case 3:
            //check bounds
            if(y > 0 && y < _height) 
                return !(*_walls)[x + (y - 1) * _width]._below;
            break;
            
        default: return false;
    }
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists){
    //to the right
    if(dir == 0){
        (*_walls)[x + y * _width]._right = exists;
    }
    //below
    else if(dir == 1){
        (*_walls)[x + y * _width]._below = exists;
    }
}

std::vector<int> SquareMaze::solveMaze(){
    //initialize variables necessary for finding solution path
    std::vector<int> solution;
    std::vector<int> path;
    std::vector<bool> visited;
    std::vector<int> prev;
    std::vector<int> dist;

    int longest_distance = 0;
    int endx = 0;
    int endy = 0;

    //go through every square in the maze. No square has been visited, so no square has a previous square.
    //distances will be set later during traversal
    for (int i = 0; i < _width * _height; i++){
        visited.push_back(false);
        prev.push_back(-1);
        dist.push_back(0);
    }

    //start at (0,0)
    path.push_back(0);
    visited[0] = true;

    while(!path.empty()){
        int currSquare = path.front();
        path.erase(path.begin()); //pop the front of our path vector
        //get current position in terms of (x,y)
        int curx = currSquare % _width;
        int cury = currSquare / _width;

        //check if we are in last row
        //if we are as far as possible from the start, this means we are at the end
        if (cury == _height - 1){
            if ((dist[curx + cury * _width] > longest_distance) || ((dist[curx + cury * _width] = longest_distance) && curx < endx)){
                longest_distance = dist[curx + cury * _width];
                endx = curx;
                endy = cury;
            }
        }
        //go through cardinal directions
        //right
        if(canTravel(curx, cury, 0) && !visited[curx + 1 + cury*_width]){
            path.push_back(curx + 1 + cury*_width);
            visited[curx + 1 + cury*_width] = true;
            prev[curx + 1 + cury*_width] = curx+ cury*_width;
            dist[curx + 1 + cury*_width] = dist[curx + cury*_width] + 1;
        }
        //down
        if(canTravel(curx, cury, 1) && !visited[curx + (cury + 1)*_width]){
            path.push_back(curx + (cury + 1)*_width);
            visited[curx + (cury + 1)*_width] = true;
            prev[curx + (cury + 1)*_width] = curx+ cury*_width;
            dist[curx + (cury + 1)*_width] = dist[curx + cury*_width] + 1;
        }
        //left
        if(canTravel(curx, cury, 2) && !visited[curx - 1 + cury*_width]){
            path.push_back(curx - 1 + cury*_width);
            visited[curx - 1 + cury*_width] = true;
            prev[curx - 1 + cury*_width] = curx+ cury*_width;
            dist[curx - 1 + cury*_width] = dist[curx + cury*_width] + 1;
        }
        //up
        if(canTravel(curx, cury, 3) && !visited[curx + (cury - 1)*_width]){
            path.push_back(curx + (cury - 1)*_width);
            visited[curx + (cury - 1)*_width] = true;
            prev[curx + (cury - 1)*_width] = curx+ cury*_width;
            dist[curx + (cury - 1)*_width] = dist[curx + cury*_width] + 1;
        }
    }

    int end = endx + endy * _width;
    std::vector<int> temp;
    //go backwards through our solution
    while (prev[end] != -1){
        temp.push_back(end);
        end = prev[end];
    }
    temp.push_back(0);
    std::reverse(temp.begin(), temp.end());
    //store the correct directions in our solution vector
    for (unsigned int i = 0; i < temp.size() - 1; i++){
        if(temp[i+1] == temp[i] + 1){
            solution.push_back(0);
        }
        else if(temp[i+1] == temp[i] + _width){
            solution.push_back(1);
        }
        else if(temp[i+1] == temp[i] - 1){
            solution.push_back(2);
        }
        else if(temp[i+1] == temp[i] - _width){
            solution.push_back(3);
        }
    }

    return solution;
}

cs225::PNG * SquareMaze::drawMaze() const{
    //create dimensions of new PNG according to Doxygen
    cs225::PNG * ret = new cs225::PNG(_width * 10 + 1, _height * 10 + 1);

    //traverse whole image
    for (int i = 0; i < _width; i++){
        for (int j = 0; j < _height; j++){
            //do not darken entrance, otherwise all outside walls are drawn
            //top wall
            if (j == 0){
                for (int k = 0; k < 10; k++){
                    if((i + k == 0) || (i * 10 + k > 9)){
                        ret->getPixel(i * 10 + k, 0).l = 0;
                    }
                }
            }
            //left wall
            if (i == 0){
                for (int k = 0; k < 10; k++){
                    ret->getPixel(0, j * 10 + k).l = 0;
                }
            }
            //now handle intermediate walls
            //right walls
            if(canTravel(i, j, 0) == false){
                for (int k = 0; k <= 10; k++){
                    ret->getPixel((i + 1) * 10, j * 10 + k).l = 0;
                }
            }

            //bottom walls
            if(canTravel(i, j, 1) == false){
                for (int k = 0; k <= 10; k++){
                    ret->getPixel(i * 10 + k, (j + 1) * 10).l = 0;
                }
            }
        }
    }
    return ret;
}

cs225::PNG * SquareMaze::drawMazeWithSolution(){
    cs225::PNG * ret = drawMaze();
    std::vector<int> solution = solveMaze();
    cs225::HSLAPixel redPixel(0, 1, 0.5, 1); //standard red hsla pixel

    //start at (5,5)
    int curx = 5;
    int cury = 5;

    //stores location so we can whiten the exit
    int endx = 0;
    int endy = 0;

    //now traverse through the solution vector
    for (auto it = solution.begin(); it != solution.end(); it++){
        //can be 1 of 4 directions. Handle each direction separately using switch statement. Each line is 10 pixels long
        switch(*it){
            //right
            case 0:
                for (int k = 0; k < 10; k++){
                    ret->getPixel(curx, cury) = redPixel;
                    curx++;
                }
                endx++;
                break;

            //down
            case 1:
                for (int k = 0; k < 10; k++){
                    ret->getPixel(curx, cury) = redPixel;
                    cury++;
                }
                endy++;
                break;

            //left
            case 2:
                for (int k = 0; k < 10; k++){
                    ret->getPixel(curx, cury) = redPixel;
                    curx--;
                }
                endx--;
                break;

            //up
            case 3:
                for (int k = 0; k < 10; k++){
                    ret->getPixel(curx, cury) = redPixel;
                    cury--;
                }
                endy--;
                break;

            default: break;
        }
    }

    ret->getPixel(curx, cury) = redPixel;

    //set exit to white pixels
    for (int k = 1; k < 10; k++){
        ret->getPixel(endx * 10 + k, (endy + 1) * 10).l = 1;
    }

    return ret;
}

//frees all newly allocated memory and gets rid of pointers associated with maze parameters
void SquareMaze::_destroy(){
    delete _walls;
    _walls = NULL;

    delete _dset;
    _dset = NULL;
}