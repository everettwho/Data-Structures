#ifndef _MAZE_H_
#define _MAZE_H_

#include "dsets.h"
#include "png.h"

#include <iostream>
#include <map>
#include <queue>
#include <stack>

using namespace std;

class SquareMaze {

    public:
    SquareMaze();
    void makeMaze(int width, int height);
    bool canTravel(int x, int y, int dir) const;
    void setWall(int x, int y, int dir, bool exist);
    vector <int> solveMaze();
    PNG * drawMaze() const;
    PNG * drawMazeWithSolution();
    
    private:
    int mazeWidth;
    int mazeHeight;
    int mazeSize;
    int exitCell;
    
    DisjointSets cells;
    vector< pair< pair<int, int>, int> >  walls;
    vector<bool> rightWall;
    vector<bool> bottomWall;
    vector <int> solution;

    int getIndex(int x, int y) const;
    int getX(int cell);
    int getY(int cell);
    int getCell(int x, int y, int dir);
};

#endif
