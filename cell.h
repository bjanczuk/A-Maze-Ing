// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// cell.h

#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <set>

enum WALL {LEFT, UP, RIGHT, DOWN};

using namespace std;

class cell {
  public:
    cell();
    ~cell();
    void setCoords(int, int);
    pair<int, int> getCoords() const;
    void switchWall(int, bool);
    bool getWall(int) const;
    void setVisited(bool);
    bool isVisited();
    void setKruskalIndex(int);
    int  getKruskalIndex();
    void setEllerIndex(int);
    int  getEllerIndex();

  private:
    pair<int, int> coords;
    bool wall[4]; // 0 = left, 1 = up, 2 = right, 3 = down
    bool visited;
    int kruskalIndex; // index of the cell in a vector of sets used for Kruskal's algorithm
    int ellerIndex; // index of the cell in a vector of sets used for Eller's algorithm
};

#endif
