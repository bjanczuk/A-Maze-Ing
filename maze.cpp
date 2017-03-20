// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// maze.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cell.h"
#include "maze.h"

using namespace std;

maze::maze()
{ }

maze::~maze()
{ }

void maze::startCell()
{
  srand(time(NULL));
  int start = rand() % 4;
  switch (start)
  {
    case 0:
      grid[0][rand() % 20].switchWall(0);
      break;
    case 1:
      grid[rand() % 20][0].switchWall(1);
      break;
    case 2:
      grid[19][rand() % 20].switchWall(2);
      break;
    case 3:
      grid[rand() % 20][19].switchWall(3);
      break;
  }
}


