// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// maze.h

#ifndef MAZE_H
#define MAZE_H

#include "cell.h"

#include <iostream>

using namespace std;

class maze {
  public:
    maze();
    ~maze();
    void startCell();

  private:
    cell grid[20][20]; // hard coded right now, but will be flexible later

};

#endif
