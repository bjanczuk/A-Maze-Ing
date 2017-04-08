// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// maze.h

#ifndef MAZE_H
#define MAZE_H

#include "cell.h"

#include <iostream>
#include <vector>

using namespace std;

class maze {
  public:
    maze(const int = 20);
    ~maze();
    bool checkBoundaries(int, int);
    pair<int, int> startCell();
    bool allVisited();
    pair<int, int> checkNeighbors(int, int);
    void removeSharedWall(pair<int, int>, pair<int, int>);
    void recursiveBack(pair<int, int>);

  private:
    int size;
    cell ** grid; // hard coded right now, but will be flexible later
    stack<cell> cellStack;
};

#endif

