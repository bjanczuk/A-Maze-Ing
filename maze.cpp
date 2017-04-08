// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// maze.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cell.h"
#include "maze.h"

using namespace std;



maze::maze(int n)
{
  cell ** grid = new cell*[n];
  for (int i = 0; i < n; i++)
    grid[i] = new cell[n];
  size = n;
}

maze::~maze()
{ 
  for (int i = 0; i < size; i++)
    delete [] grid[i];
  delete [] grid;
}

void maze::startCell()
{
  srand(time(NULL));
  int start = rand() % 4;
  switch (start)
  {
    case 0:
      grid[0][rand() % size].switchWall(0);
      break;
    case 1:
      grid[rand() % size][0].switchWall(1);
      break;
    case 2:
      grid[size-1][rand() % size].switchWall(2);
      break;
    case 3:
      grid[rand() % size][size - 1].switchWall(3);
      break;
  }
}

void maze::checkNeighbors()
{

}

