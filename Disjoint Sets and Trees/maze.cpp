#include "maze.h"

#define RIGHT 0
#define BOTTOM 1
#define LEFT 2
#define UP 3

SquareMaze::SquareMaze()
{
    mazeWidth = 1;
    mazeHeight = 1;
    mazeSize = 1;
    cells = DisjointSets();
}

void SquareMaze::makeMaze(int width, int height)
{
	if (width < 2 || height < 2) {return;}  
	
    mazeWidth = width;
    mazeHeight = height;
    mazeSize = mazeWidth * mazeHeight;

    srand (time(NULL));
    cells.addelements(mazeSize);

    rightWall = vector<bool> (mazeSize, true);
    bottomWall = vector<bool> (mazeSize, true);
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            pair<int, int> point (i, j);
            pair< pair<int, int> , int > right (point, RIGHT);
            pair< pair<int, int> , int > bottom (point, BOTTOM);

            walls.push_back(right);
            walls.push_back(bottom);
        }
    }

    while (walls.size() != 1)
    {
        int wallIndex = rand() % walls.size();
        
        pair < pair<int, int>, int > wall = walls[wallIndex];
        walls[wallIndex] = walls[walls.size() - 1];
        walls.pop_back();
        
        pair <int, int > point = wall.first;
        
        int direction = wall.second;
        int x = point.first;
        int y = point.second;
        
        int cellIndex = getIndex(x, y);

        if (direction) {
            if (cellIndex + width >= mazeSize) {continue;}
            if (cells.find(cellIndex) == cells.find(cellIndex + width)) {continue;}

            cells.setunion(cellIndex, cellIndex + width);
            bottomWall[cellIndex] = false;
        } 
        else {
            if (((cellIndex + 1) % width) == 0) {continue;}
            if (cells.find(cellIndex) == cells.find(cellIndex + 1)) {continue;}

            cells.setunion( cellIndex, cellIndex +1);
            rightWall[cellIndex] = false;
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const
{
    if ( x < 0 || y < 0 || x > mazeWidth || y > mazeHeight ) {return false;}
    if ((x == mazeWidth -1 && dir == RIGHT) || (y == mazeHeight -1 && dir == BOTTOM) || (x == 0 && dir == LEFT) || (y == 0 && dir == UP)) {return false;}
       
    switch (dir)
    {
        case RIGHT:
            return !(rightWall[getIndex(x, y)]);
        case BOTTOM:
            return !(bottomWall[getIndex(x, y)]);
        case LEFT:
            return !(rightWall[getIndex(x-1, y)]);
        case UP:
            return !(bottomWall[getIndex(x, y-1)]);
        default:
            return false;
    }
}

void SquareMaze::setWall (int x, int y, int dir, bool exists)
{
    int cellIndex = getIndex(x, y);

    if (dir == RIGHT) {rightWall[cellIndex] = exists;}
    else {bottomWall[cellIndex] = exists;}
}

vector <int> SquareMaze::solveMaze()
{
    map <int, int> path;
    bool visitTrack[mazeWidth][mazeHeight];
    
    for (int i = 0; i < mazeWidth; i++) {
        for (int j = 0; j < mazeHeight; j++) {
            visitTrack[i][j] = false;
        }
    }

    queue <int> neighbors;
    neighbors.push(0);
    visitTrack[0][0] = true;

    while (!neighbors.empty())
    {
        int current = neighbors.front();
        neighbors.pop();
                
        int x = getX(current);
        int y = getY(current);

        for (int dir = 0; dir < 4; dir++)
        {
            int next = getCell(x, y, dir);
            int nextX = getX(next);
            int nextY = getY(next);

            if (canTravel(x, y, dir) && !visitTrack[nextX][nextY])
            {
                neighbors.push(next);
                visitTrack[nextX][nextY] = true;
                path[next] = current;
            } 
        }
    }

    int length= 0;
    int index = 0;
    
    for (int i = 0; i < mazeWidth; i++) {
        int stepCount = 0;
        int start = 0;
        int end = getIndex(i, mazeHeight -1);

        while (end != start) {
            end = path[end];
            stepCount++;
        }

        if (stepCount > length) {
            length = stepCount;
            index = i;
        }
    }
    
    exitCell = getIndex(index, mazeHeight - 1);

    int start = 0;
    int end = exitCell;

    stack<int> steps;
    while (end != start) {
        int diff = end - path[end];
        int dir = -1;
        
        if (diff == 1) {dir = 0;}
        else if (diff == mazeWidth) {dir = 1;}
        else if (diff == -1) {dir = 2;}
        else if (diff == ((-1) * mazeWidth)) {dir = 3;}
        
        steps.push(dir);
        end = path[end];
    }
    
    while (!steps.empty()) {   
        if (steps.top() == -1) {return solution;}
        solution.push_back(steps.top());
        steps.pop();
    }

    return solution;    
}

PNG * SquareMaze::drawMaze() const
{
    int x, y;
	int pixelWidth = 10 * mazeWidth + 1;
	int pixelHeight = 10 * mazeHeight + 1;
	
    PNG * maze = new PNG(pixelWidth, pixelHeight);

    for (int i = 10; i < pixelWidth; i++) {
        (*maze)(i, 0)->red = 0;
        (*maze)(i, 0)->green = 0;
        (*maze)(i, 0)->blue = 0;
    }

    for (int i = 0; i < pixelHeight; i++ )
    {
        (*maze)(0, i)->red = 0;
        (*maze)(0, i)->green = 0;
        (*maze)(0, i)->blue = 0;
    }

    for (int i = 0; i < mazeWidth; i++) {
        for (int j = 0; j < mazeHeight; j++) {
            if (bottomWall[getIndex(i, j)] ) {
                for (int k = 0; k <= 10; k++) {
                    x = i * 10 + k;
                    y = (j + 1) * 10;

                    (*maze)(x, y)->red = 0;
                    (*maze)(x, y)->green = 0;
                    (*maze)(x, y)->blue = 0;
                }
            }

            if (rightWall[getIndex(i, j)]) {
                for (int k = 0; k <= 10; k++) {
                    x = (i + 1) * 10;
                    y = j * 10 + k;

                    (*maze)(x, y)->red = 0;
                    (*maze)(x, y)->green = 0;
                    (*maze)(x, y)->blue = 0;
                }
            }
        }
    }

    return maze;    
}
PNG * SquareMaze::drawMazeWithSolution()
{
    if (solution.size() == 0) {return new PNG();}
    PNG * maze = drawMaze();
    
    int x = 5;
    int y = 5;

    for (int i = 0; i < solution.size(); i++) {
        if (solution[i] == RIGHT) {
            for (int j = 0; j <= 10; j++ ) {
                (*maze)(x + j, y)->red = 255;
                (*maze)(x + j, y)->blue = 0;
                (*maze)(x + j, y)->green = 0;
            }
            x += 10;
        } 
        else if ( solution [i] == BOTTOM) {
            for (int j = 0; j <= 10; j++) {
                (*maze)(x, y + j)->red = 255;
                (*maze)(x, y + j)->blue = 0;
                (*maze)(x, y + j)->green = 0;
            }
            y += 10;
        }
        else if ( solution [i] == LEFT) {
            for (int j = 0; j <= 10; j++) {
                (*maze)(x - j, y)->red = 255;
                (*maze)(x - j, y)->blue = 0;
                (*maze)(x - j, y)->green = 0;
            }
            x -= 10;
        }
        else if ( solution [i] == UP) {
            for (int j = 0; j <= 10; j++) {
                (*maze)(x, y - j)->red = 255;
                (*maze)(x, y - j)->blue = 0;
                (*maze)(x, y - j)->green = 0;
            }
            y -= 10;
        }
    }

    int endX = x - 5;
    int endY = y + 5;

    for (int i = 1; i < 10; i++) {
        (*maze)(endX + i, endY)->red = 255;
        (*maze)(endX + i, endY)->green = 255;
        (*maze)(endX + i, endY)->blue = 255;
    }

    return maze; 
}

int SquareMaze::getIndex(int x, int y) const {return (y * mazeWidth + x);}
int SquareMaze::getX(int cell) {return (cell % mazeWidth);}
int SquareMaze::getY(int cell){return (cell / mazeWidth);}

int SquareMaze::getCell(int x, int y, int dir)
{
    switch(dir)
    {
        case RIGHT:
            return getIndex(x, y) + 1;
        case LEFT:
            return getIndex(x, y) - 1;
        case BOTTOM:
            return getIndex(x, y) + mazeWidth;
        case UP:
            return getIndex(x, y) - mazeWidth;
        default: return 0;
    }
}
