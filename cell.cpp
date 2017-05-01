// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// cell.cpp

#include "cell.h"

#include <iostream>

using namespace std;

/* Cell constructor */
cell::cell()
{
  for (int i = 0; i < 4; i++)
  {
    wall[i] = true;
  }
  visited = false;
}

/* Cell deconstructor */
cell::~cell() {}

/* Set the cell's coordinates within the grid */
void cell::setCoords(int x, int y)
{
  coords.first = x;
  coords.second = y;
}

/* Return the cell's coordinates as a pair */
pair<int, int> cell::getCoords() const
{
  return coords;
}

/* Change the cell's "i" wall to a value of "on" */
void cell::switchWall(int i, bool on)
{
  wall[i] = on;
}

/* Return whether the cell's "i" wall is on or off */
bool cell::getWall(int i) const
{
  return wall[i];
}

/* Set the cell to a value of "visit" */
void cell::setVisited(bool visit)
{
  visited = visit;
}

/* Return whether the cell has been visited */
bool cell::isVisited()
{
  return visited;
}

/* Set the cell's index in the vector used for Kruskal's algorithm */
void cell::setKruskalIndex(int set)
{
  kruskalIndex = set;
}

/* Return the cell's index in the vector used for Kruskal's algorithm */
int cell::getKruskalIndex()
{
  return kruskalIndex;
}

/* Set the cell's index in the vector used for Eller's algorithm */
void cell::setEllerIndex(int set)
{
  ellerIndex = set;
}

/* Return the cell's index in the vector used for Eller's algorithm */
int cell::getEllerIndex()
{
  return ellerIndex;
}