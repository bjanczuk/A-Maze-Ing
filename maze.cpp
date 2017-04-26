// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// maze.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <vector>
#include "cell.h"
#include "maze.h"

const int LINESIZE = 30;

using namespace std;

maze::maze(SDL_Renderer* renderer, int n)
{
  cell tempCell;
  vector<cell> tempVec(n, tempCell);
  for (int i = 0; i < n; i++)
  {
    grid.push_back(tempVec);
    for (int j = 0; j < n; j++) // set the new cell coordinates
    {
    	grid[i][j].setCoords(i, j);
	drawBox(i, j, renderer);
    }
    cout << grid.size() << " " << grid[i].size();
    SDL_RenderPresent(renderer);
  }
  size = n;
}

maze::~maze()
{ }

bool maze::checkBoundaries(int r, int c)
{
  return (r >= 0 && c >= 0 && r < size && c < size);
}

pair<int, int> maze::startCell()
{
  srand(time(NULL));
  int start = rand() % 4;
  int randCell = rand() % size;
  switch (start)
  {
    case 0:
      grid[0][randCell].switchWall(0, true);
      return make_pair(0, randCell);
      break;
    case 1:
      grid[randCell][0].switchWall(1, true);
      return make_pair(randCell, 0);
      break;
    case 2:
      grid[size-1][randCell].switchWall(2, true);
      return make_pair(size-1, randCell);
      break;
    default: // effectively case 3
      grid[randCell][size - 1].switchWall(3, true);
      return make_pair(randCell, size-1);
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

vector<pair<int, int>> maze::getNeighbors(int r, int c)
{
  vector<pair<int, int>> neighbors; // vector of the neighbors for the cell at r,c
  
  // add the four neighbors to the vector
  neighbors.emplace_back(r-1, c); neighbors.emplace_back(r+1, c);
  neighbors.emplace_back(r, c-1); neighbors.emplace_back(r, c+1);

  for (auto it = neighbors.begin(); it != neighbors.end(); it++)
  {
    if (!checkBoundaries((*it).first, (*it).second))
      neighbors.erase(it); // remove any cell that's out of bounds
  }

  return neighbors;
}

pair<int, int> maze::removeVisitedNeighbors(int r, int c)
{
  vector<pair<int, int>> neighbors = getNeighbors(r, c); // vector of the cell's neighbors
  
  // check each neighbor
  for (auto it = neighbors.begin(); it != neighbors.end(); it++)
  {
    if (grid[(*it).first][(*it).second].isVisited())
    {
      neighbors.erase(it); // remove a cell if it has been visited
    }
  }
  
  return (neighbors.size()) ? neighbors[rand() % neighbors.size()] : make_pair(-1, -1); // randomly return one of the non-visited neighbors
}

void maze::removeSharedWall(pair<int, int> x, pair<int, int> y, SDL_Renderer* renderer)
{ 
  if (x.first > y.first) // x is beneath y
  {
    grid[x.first][x.second].switchWall(UP, false);
    grid[y.first][y.second].switchWall(DOWN, false);
    drawWall(x.first, x.second, UP, renderer);
  }
  else if (x.first < y.first) // x is above y
  {
    grid[x.first][x.second].switchWall(DOWN, false);
    grid[y.first][y.second].switchWall(UP, false);
    drawWall(x.first, x.second, DOWN, renderer);
  }
  else if (x.second > y.second) // x is to the right of y
  {
    grid[x.first][x.second].switchWall(LEFT, false);
    grid[y.first][y.second].switchWall(RIGHT, false);
    drawWall(x.first, x.second, LEFT, renderer);
  }
  else if (x.second < y.second) // x is to the left of y
  {
    grid[x.first][x.second].switchWall(RIGHT, false);
    grid[y.first][y.second].switchWall(LEFT, false);
    drawWall(x.first, x.second, RIGHT, renderer);
  }
}

void maze::recursiveBack(pair<int, int> start, SDL_Renderer* renderer) 
{
    cell curr = grid[start.first][start.second];
    curr.setVisited(true);

    if (allVisited())
        return;
    
    pair<int, int> neigh = removeVisitedNeighbors(start.first, start.second);
    if (neigh.first >= 0) // make sure a neighbor was found
    {
        cellStack.push(curr);
        removeSharedWall(start, neigh, renderer);
        start = neigh;
        recursiveBack(start, renderer);
    }
    else if (!cellStack.empty())
    {
        start = cellStack.top().getCoords();
        cellStack.pop();
        recursiveBack(start, renderer);
    }
    else
    {
        return;
    }
}

void maze::prims(pair<int,int> start, SDL_Renderer* renderer)
{
    cell curr = grid[start.first][start.second];
    curr.setVisited(true);
    auto it = frontier.begin();
    int index, currIndex;

    vector<pair<int, int>> neighbors = getNeighbors(curr.getCoords().first, curr.getCoords().second);
    addToFrontier(neighbors); // set up the initial frontier

    while (frontier.size()) // iterate until the frontier is empty
    {
      it = frontier.begin();
      advance(it, rand() % frontier.size()); // randomly choose a frontier cell
      
      curr = grid[(*it).first][(*it).second];
      cout << frontier.size() << endl;
      cout << "\t" << (*it).first << " " << (*it).second << " ";
      curr.setVisited(true);
      frontier.erase(it); // remove the cell from the frontier

      neighbors = getNeighbors(curr.getCoords().first, curr.getCoords().second); // get the new cell's neighbors
      addToFrontier(neighbors);
      index = rand() % neighbors.size(); // start at a random neighbor
      currIndex = index;
      cout << neighbors.size() << " " << currIndex << endl;

      do // look for a neighbor that's visited
      {
        if (grid[neighbors[currIndex].first][neighbors[currIndex].second].isVisited()) // a visited cell was found, so break
          break;
        currIndex++;
        currIndex %= neighbors.size(); // make sure the index is in bounds
      }
      while (currIndex != index);

      removeSharedWall(grid[neighbors[currIndex].first][neighbors[currIndex].second].getCoords(), curr.getCoords(), renderer);
    }
}

void maze::addToFrontier(vector<pair<int, int>> neighbors)
{
    for (auto it = neighbors.begin(); it != neighbors.end(); it++)
    {
      cout << "adding to frontier: " << (*it).first << " " << (*it).second << endl;
      if (grid[(*it).first][(*it).second].isVisited() == false)
        frontier.insert(*it);
    }
}

void maze::drawBox(int i, int j, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  //format: SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  SDL_RenderDrawLine(renderer, i*LINESIZE, j*LINESIZE, i*LINESIZE, j*LINESIZE + 30); // left
  SDL_RenderDrawLine(renderer, i*LINESIZE, j*LINESIZE, i*LINESIZE + LINESIZE, j*LINESIZE); // up
  SDL_RenderDrawLine(renderer, i*LINESIZE + LINESIZE, j*LINESIZE, i*LINESIZE + LINESIZE, j*LINESIZE + 30); // right
  SDL_RenderDrawLine(renderer, i*LINESIZE, j*LINESIZE + 30, i*LINESIZE + LINESIZE, j*LINESIZE + 30); // down
}

void maze::drawWall(int j, int i, int wall, SDL_Renderer* renderer) {
  cout << "i = " << i << " j = " << j << endl;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  switch (wall) {
    case LEFT:
      cout << "LEFT" << endl;
      SDL_RenderDrawLine(renderer, i*LINESIZE, j*LINESIZE, i*LINESIZE, j*LINESIZE + LINESIZE); // left
      break;
    case UP:
      cout << "UP" << endl;
      SDL_RenderDrawLine(renderer, i*LINESIZE, j*LINESIZE, i*LINESIZE + LINESIZE, j*LINESIZE); // up
      break;
    case RIGHT:
      cout << "RIGHT" << endl;
      SDL_RenderDrawLine(renderer, i*LINESIZE + LINESIZE, j*LINESIZE, i*LINESIZE + LINESIZE, j*LINESIZE + LINESIZE); // right
      break;
    case DOWN:
      cout << "DOWN" << endl;
      SDL_RenderDrawLine(renderer, i*LINESIZE, j*LINESIZE + LINESIZE, i*LINESIZE + LINESIZE, j*LINESIZE + LINESIZE); // down
      break;
  }
  
  SDL_RenderPresent(renderer);
  usleep(20000000);
}

