// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// cell.h

#ifndef CELL_H
#define CELL_H

#include <iostream>

enum WALL {LEFT, UP, RIGHT, DOWN};

using namespace std;

class cell {
  public:
    cell(); // constructor
    ~cell(); // destructor
    void setVisited(bool);
    void switchWall(int, bool);
    bool getWall(int);
    bool isVisited();
    void setCoords(int, int);
    pair<int, int> getCoords();

  private:
    bool wall[4]; // 0 = left, 1 = up, 2 = right, 3 = down
    bool visited;
    pair<int, int> coords;
};

#endif
