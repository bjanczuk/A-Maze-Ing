// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// maze.h

#ifndef MAZE_H
#define MAZE_H

#include "cell.h"

#include <iostream>

using namespace std;

class maze {
  public:
    maze(const int = 20);
    ~maze();
    void startCell();
    void checkNeighbors();

  private:
    int size;
    cell ** grid; // hard coded right now, but will be flexible later

};

#endif
