// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// maze.h

#ifndef MAZE_H
#define MAZE_H

#include "cell.h"

#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

class maze {
  public:
    maze(SDL_Renderer*, const int = 20);
    ~maze();
    bool checkBoundaries(int, int);
    pair<int, int> startCell();
    bool allVisited();
    vector<pair<int, int>> getNeighbors(int, int);
    pair<int, int> removeVisitedNeighbors(int, int);
    void removeSharedWall(pair<int, int>, pair<int, int>, SDL_Renderer*);
    void addToFrontier(vector<pair<int, int>>);
    void recursiveBack(pair<int, int>, SDL_Renderer*);
    void prims(pair<int, int>, SDL_Renderer*);
    void drawBox(int, int, SDL_Renderer*);
    void drawWall(int, int, int, SDL_Renderer*);

  private:
    int size;
    vector<vector<cell>> grid;
    //cell ** grid; // hard coded right now, but will be flexible later
    stack<cell> cellStack;
    set<pair<int, int>> frontier;
};

#endif


