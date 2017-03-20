// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// cell.cpp

#include "cell.h"

#include <iostream>

using namespace std;

cell::cell()
{
  for (int i = 0; i < 4; i++)
  {
    wall[i] = true;
  }
  visited = false;
}

cell::~cell()
{ }

void cell::switchWall(int i)
{
  wall[i] = !wall[i];
}

bool cell::getWall(int i)
{
  return wall[i];
}

bool cell::isVisited()
{
  return visited;
}

