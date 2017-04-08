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
  {
    grid[i] = new cell[n];
  }
  size = n;
}

maze::~maze()
{ 
  for (int i = 0; i < size; i++)
    delete [] grid[i];
  delete [] grid;
}

bool maze::checkBoundaries(int r, int c)
{
	return (r >= 0 && c >= 0 && r < size && c < size);
}

void maze::startCell()
{
  srand(time(NULL));
  int start = rand() % 4;
  switch (start)
  {
    case 0:
      grid[0][rand() % size].switchWall(0, false);
      break;
    case 1:
      grid[rand() % size][0].switchWall(1, false);
      break;
    case 2:
      grid[size-1][rand() % size].switchWall(2, false);
      break;
    case 3:
      grid[rand() % size][size - 1].switchWall(3, false);
      break;
  }
}

bool maze::allVisited()
{
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (!(grid[i][j].isVisited()))
        return false;
    }
  }
  return true;
}

pair<int, int> maze::checkNeighbors(int r, int c)
{
  vector<pair<int, int>> neighbors; // vector of the unvisited neighbors for the cell at r,c
  neighbors.emplace_back(r-1, c); neighbors.emplace_back(r+1, c); neighbors.emplace_back(r, c-1); neighbors.emplace_back(r, c+1); // add the four neighbors to the vector
  
  // check each neighbor
  for (auto it = neighbors.begin(); it != neighbors.end(); it++)
  {
    if (!checkBoundaries((*it).first, (*it).second) || grid[(*it).first][(*it).second].isVisited())
    {
      neighbors.erase(it); // remove a cell if it's either out of bounds or has been visited
    }
  }
  
  return (neighbors.size()) ? neighbors[rand() % neighbors.size()] : make_pair(-1, -1); // randomly return one of the non-visited neighbors
}

void maze::removeSharedWall(pair<int, int> x, pair<int, int> y)
{
  if (x.first > y.first) // x is beneath y
  {
    grid[x.first][x.second].switchWall(UP, false);
    grid[y.first][y.second].switchWall(DOWN, false); 
  }
  else if (x.first < y.first) // x is above y
  {
    grid[x.first][x.second].switchWall(DOWN, false);
    grid[y.first][y.second].switchWall(UP, false);
  }
  else if (x.second > y.second) // x is to the right of y
  {
    grid[x.first][x.second].switchWall(LEFT, false);
    grid[y.first][y.second].switchWall(RIGHT, false);
  }
  else if (x.second < y.second) // x is to the left of y
  {
    grid[x.first][x.second].switchWall(RIGHT, false);
    grid[y.first][y.second].switchWall(LEFT, false);
  }
}
