// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// maze.cpp

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <time.h>
#include <SDL2/SDL.h>
#include <vector>
#include "cell.h"
#include "maze.h"

using namespace std;

const int LINESIZE = 30; // determines how big each side of a cell is
int WAIT; // determines how long the program should wait after drawing lines

/* Maze constructor */
maze::maze(SDL_Renderer* renderer, int n, int wait, bool draw)
{
  cell tempCell;
  vector<cell> tempVec(n, tempCell);
  for (int i = 0; i < n; i++)
  {
    grid.push_back(tempVec);
    for (int j = 0; j < n; j++) // initialize each cell
    {
    	grid[i][j].setCoords(i, j);  // set the new cell coordinates

      if (draw) // initialize as on, and draw the cell, to begin with for all but Eller's algorithm
      {
	     drawBox(i, j, renderer);
       for (int k = 0; k < 4; k++)
          grid[i][j].switchWall(k, true);
      }
      else
      {
        grid[i][j].setEllerIndex(-1); // otherwise initialize each cell as off
        for (int k = 0; k < 4; k++)
          grid[i][j].switchWall(k, false);
      }
    }
    SDL_RenderPresent(renderer);
  }
  size = n; // update the size 
  WAIT = wait; // update the "WAIT" global variable
}

/* Maze deconstructor */
maze::~maze() {}

 /* Choose a random cell on the border as the "starting" cell */
pair<int, int> maze::startCell()
{
  srand(time(NULL));
  int start = rand() % 4;
  int randCell = rand() % size;
  switch (start)
  {
    case 0:
      grid[0][randCell].switchWall(0, false);
      return make_pair(0, randCell);
      break;
    case 1:
      grid[randCell][0].switchWall(1, false);
      return make_pair(randCell, 0);
      break;
    case 2:
      grid[size-1][randCell].switchWall(2, false);
      return make_pair(size-1, randCell);
      break;
    default: // effectively case 3
      grid[randCell][size - 1].switchWall(3, false);
      return make_pair(randCell, size-1);
      break;
  }
}

/* Return a vector of the coordinates of each valid cell neighboring the cell at (r, c) */
vector<pair<int, int>> maze::getNeighbors(int r, int c)
{
  vector<pair<int, int>> neighbors;
  
  // initially add all four neighbors to the vector
  neighbors.emplace_back(r-1, c); neighbors.emplace_back(r+1, c);
  neighbors.emplace_back(r, c-1); neighbors.emplace_back(r, c+1);

  for (auto it = neighbors.begin(); it != neighbors.end();)
  {
    if (checkBoundaries((*it).first, (*it).second))
      it++;
    else
      neighbors.erase(it); // remove any cell that's out of bounds
  }
  return neighbors;
}

/* Check whether (r, c) are valid coordinates */
bool maze::checkBoundaries(int r, int c)
{
  return (r >= 0 && c >= 0 && r < size && c < size);
}

/* Update the shared wall between two cells to on/off in each of their "wall" arrays
   Draw a white or black line to reflect the change in the SDL window */
void maze::changeSharedWall(bool on, pair<int, int> x, pair<int, int> y, SDL_Renderer* renderer)
{ 
  int color;
  if (on) // draw a white line if the wall is "on", black if not
  	color = 255;
  else
  	color = 0;

  if (x.first > y.first) // x is beneath y
  {
    grid[x.first][x.second].switchWall(UP, on);
    grid[y.first][y.second].switchWall(DOWN, on);
    drawWall(x.first, x.second, color, UP, renderer);
  }
  else if (x.first < y.first) // x is above y
  {
    grid[x.first][x.second].switchWall(DOWN, on);
    grid[y.first][y.second].switchWall(UP, on);
    drawWall(x.first, x.second, color, DOWN, renderer);
  }
  else if (x.second > y.second) // x is to the right of y
  {
    grid[x.first][x.second].switchWall(LEFT, on);
    grid[y.first][y.second].switchWall(RIGHT, on);
    drawWall(x.first, x.second, color, LEFT, renderer);
  }
  else if (x.second < y.second) // x is to the left of y
  {
    grid[x.first][x.second].switchWall(RIGHT, on);
    grid[y.first][y.second].switchWall(LEFT, on);
    drawWall(x.first, x.second, color, RIGHT, renderer);
  }
}

/* Use the recursive backtracking algorithm to generate a maze */
void maze::recursiveBack(pair<int, int> start, SDL_Renderer* renderer) 
{
    cell curr = grid[start.first][start.second];
    grid[start.first][start.second].setVisited(true);

    if (allVisited())
        return;
    
    pair<int, int> neigh = getRandomUnvisitedNeighbor(start.first, start.second); // get a random unvisited neighbor
    if (neigh.first >= 0) // make sure a valid neighbor was found
    {
        // if so, remove the shared wall between the current cell and the neighbor and recurse
        cellStack.push(curr);
        changeSharedWall(false, start, neigh, renderer);
        start = neigh;
        recursiveBack(start, renderer); // recurse
    }
    else if (!cellStack.empty()) // if not, take the top cell from the stack (if there is one) and recurse
    {
        start = cellStack.top().getCoords();
        cellStack.pop();
        recursiveBack(start, renderer); // recurse
    }
    else // return if every neighbor has been visited and the stack is empty
    {
        return; // base case
    }
}

/* Return whether all the cells in the grid have been visited */
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

/* Return the coordinates for a random, unvisited neighbor of the cell at (r, c)
   Return coordinates of (-1, -1) if all of the neighbors have been visited */
pair<int, int> maze::getRandomUnvisitedNeighbor(int r, int c)
{
  vector<pair<int, int>> neighbors = getNeighbors(r, c); // vector of the cell's neighbors
  
  // check each neighbor
  for (auto it = neighbors.begin(); it != neighbors.end();)
  {
    if (grid[(*it).first][(*it).second].isVisited())
    {
      neighbors.erase(it); // remove a cell if it has been visited
    }
    else
    {
      it++;
    }
  }
  return (neighbors.size()) ? neighbors[rand() % neighbors.size()] : make_pair(-1, -1); // randomly return one of the non-visited neighbors
}

/* Use Prim's algorithm to generate a maze */
void maze::prims(pair<int,int> start, SDL_Renderer* renderer)
{
    int neighborIndex;
    set<pair<int, int>>::iterator it;

    // get the very first cell and set it to visited
    cell curr = grid[start.first][start.second];
    grid[start.first][start.second].setVisited(true);

    vector<pair<int, int>> neighbors = getNeighbors(curr.getCoords().first, curr.getCoords().second);
    addToFrontier(neighbors); // set up the frontier given the starting cell

    while (frontier.size()) // iterate until the frontier is empty
    {
      it = frontier.begin();
      advance(it, rand() % frontier.size()); // randomly choose a frontier cell
      curr = grid[(*it).first][(*it).second];

      grid[(*it).first][(*it).second].setVisited(true); // set this cell to visited and remove it from the frontier
      frontier.erase(it);

      // get the new cell's neighbors and add them to the frontier
      neighbors = getNeighbors(curr.getCoords().first, curr.getCoords().second);
      addToFrontier(neighbors);

      // get a random visited neighbor and remove the shared wall
      neighborIndex = getRandomVisitedNeighbor(neighbors);
      changeSharedWall(false, grid[neighbors[neighborIndex].first][neighbors[neighborIndex].second].getCoords(), curr.getCoords(), renderer);
    }
}

/* Add all of the non-visited cells in the "neighbors" vector to the frontier set */
void maze::addToFrontier(vector<pair<int, int>> neighbors)
{
    for (auto it = neighbors.begin(); it != neighbors.end(); it++)
    {
      if (grid[(*it).first][(*it).second].isVisited() == false) // check if the cell's been visited
        frontier.insert(*it);
    }
}

/* Return the index of a random visited cell in the "neighbors" vector */
int maze::getRandomVisitedNeighbor(vector<pair<int, int>> neighbors)
{
  int index = rand() % neighbors.size(); // start at a random neighbor
  int neighborIndex = index;

  do // iterate through the neighbors, looking for one that's visited
  {
    if (grid[neighbors[neighborIndex].first][neighbors[neighborIndex].second].isVisited()) // a visited cell was found, so break
      break;

    neighborIndex++;
    neighborIndex %= neighbors.size(); // keep the index in bounds
  }
  while (neighborIndex != index);

  return neighborIndex;
}

/* Use Kruskal's algorithm to generate a maze */
void maze::kruskals(SDL_Renderer* renderer)
{
  int counter = 0;
  vector<pair<pair<int, int>, pair<int, int>>> edges;
  
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++, counter++) // insert each cell's coordinates into the set 
    {
      set<pair<int, int>> tempSet;
      tempSet.insert(grid[i][j].getCoords());
      sets.push_back(tempSet);

      addNeighborEdges(edges, i, j); // also add the cell's neighbors into "edges"
      grid[i][j].setKruskalIndex(counter); // update the vector index variable for each cell
    }
  }

  // use "algorithm" to randomly shuffle the "edges" vector
  random_shuffle(edges.begin(), edges.end());

  while (edges.size()) // keep building the maze until all of the edges have been popped
  {
    // get the last edge in the vector, then pop it
    pair<pair<int, int>, pair<int, int>> temp = edges.back();
    edges.pop_back();

    int x  = grid[temp.first.first][temp.first.second].getKruskalIndex(); // get the set for the first cell
    int y = grid[temp.second.first][temp.second.second].getKruskalIndex(); // get the set for the second cell

    if (x != y) // if the two cells aren't in the same set
    {
      // join the sets
      sets[x].insert(sets[y].begin(), sets[y].end());

      for (auto it = sets[x].begin(); it != sets[x].end(); it++) // update the index of each value in this set
        grid[(*it).first][(*it).second].setKruskalIndex(x);

      changeSharedWall(false, temp.first, temp.second, renderer); // remove the shared wall between the first and second cell
    }
  }
  sets.clear(); // clear the vector of all the cell's sets
}

/* Update the "edges" vector by adding each edge touching the cell at (r, c) */
void maze::addNeighborEdges(vector<pair<pair<int, int>, pair<int, int>>> &edges, int r, int c)
{
  const pair<int, int> x = make_pair(r, c);
  vector<pair<int, int>> neighbors = getNeighbors(r, c);

  // make a pair of pairs to represent each edge between the cell at (r, c) and its neighbors
  for (auto it = neighbors.begin(); it != neighbors.end(); it++)
    edges.push_back(make_pair(x, *it)); // push that pair of pairs to the "edges" vector
}

/* Use Eller's algorithm to generate a maze */
void maze::ellers(SDL_Renderer* renderer)
{
	createBorder(renderer); // draw the outer walls on each side
	
	// initialize row sets
	set<pair<int, int>> tempSet;
	for (unsigned int i = 0; i < grid.size(); i++)
		row.push_back(tempSet);

	// implement the algorithm for all but the last row
	for (int i = 0; i < (int)grid.size() - 1; i++)
	{
		updateUniqueSets(i);
		randomizeRightWalls(i, renderer);
		randomizeBottomWalls(i, renderer);

		if (i + 1 != (int)grid.size() - 1) // initialize the next row for all but the bottom row
			initializeNextRow(i + 1);
    else
      initializeLastRow(i + 1);
	}

	implementLastRow(grid.size() - 1, renderer); // implement the algorithm for the last row
	row.clear(); // clear the vector of each row's sets
}

/* Iterate through each cell in the "r-th" row and give it its own set if it doesn't already have one */
void maze::updateUniqueSets(int r)
{
  set<int> indeces;
  int index = 0;

  for (int i = 0; i < (int)grid.size(); i++)
  {
    if (grid[r][i].getEllerIndex() != -1) // store the sets that are already being used
    {
      indeces.insert(grid[r][i].getEllerIndex());
    }
  }

	for (int i = 0; i < (int)grid.size(); i++)
	{
		if (grid[r][i].getEllerIndex() == -1)
		{
      while (indeces.find(index) != indeces.end()) // find the lowest non-used set and add this cell to it
        index++;

      grid[r][i].setEllerIndex(index); // update this cell's set index
			row[index].insert(grid[r][i].getCoords()); // add this cell to that set
      indeces.insert(index); // update the set of indeces with this new index
		}
	}
}

/* Use Eller's algorithm to randomly create vertical walls between cells in the "r-th" row */
void maze::randomizeRightWalls(int r, SDL_Renderer* renderer)
{
	for (int i = 0; i < (int)(grid.size()) - 1; i++)
	{
    // if adjacent cells are in the same set, always add a wall between them
		if (grid[r][i].getEllerIndex() == grid[r][i+1].getEllerIndex())
		{
			changeSharedWall(true, grid[r][i].getCoords(), grid[r][i+1].getCoords(), renderer);
		}
		else
		{
			if (rand() % 2) // randomly add a wall between adjacent cells of different sets 1/2 of the time
			{
				changeSharedWall(true, grid[r][i].getCoords(), grid[r][i+1].getCoords(), renderer);
			}
			else // otherwise union the sets of the two adjacent cells
			{
				set<pair<int, int>> set1 = row[grid[r][i].getEllerIndex()];
				set<pair<int, int>> set2 = row[grid[r][i+1].getEllerIndex()];
				set1.insert(set2.begin(), set2.end());

        row[grid[r][i+1].getEllerIndex()].clear(); // clear the set that got merged
        updateAllCellsInSet(set2, grid[r][i].getEllerIndex()); // update the index of all the cells that are being merged

        row[grid[r][i].getEllerIndex()] = set1;
			}
		}
	}
}

/* Update the set index of every cell in this set */
void maze::updateAllCellsInSet(set<pair<int, int>> s, int index)
{
  for (auto it = s.begin(); it != s.end(); it++)
    grid[it->first][it->second].setEllerIndex(index);
}

/* Use Eller's algorithm to randomly create horizontal walls between cells in the "r-th" row and the cells beneath them */
void maze::randomizeBottomWalls(int r, SDL_Renderer* renderer)
{
	set<pair<int, int>> tempSet;
  int bottomless;

	for (unsigned int i = 0; i < grid.size(); i++)
	{
		tempSet = row[grid[r][i].getEllerIndex()];

		if (tempSet.size() > 1) // only adjust cells that aren't alone in their set
		{
      bottomless = countBottomlessInSet(tempSet); // count the number of cells in this set that don't have a bottom wall

      // if this cell is not the only bottomless one in its set, randomly add a wall beneath it 1/2 of the time
			if (!(grid[r][i].getWall(DOWN) == false && bottomless == 1))
			{
				if (rand() % 2)
					changeSharedWall(true, grid[r][i].getCoords(), grid[r+1][i].getCoords(), renderer);
			}
		}
	}
}

/* Return the number of cells in the set "s" that don't have a bottom wall */
int maze::countBottomlessInSet(set<pair<int, int>> s)
{
	int count = 0;
  int index = 0;
  
	for (auto it = s.begin(); it != s.end(); it++, index++)
	{
		if (grid[it->first][it->second].getWall(DOWN) == false) // check if the bottom wall for this cell is off
			count++;
	}
	return count;
}

/* Clear the "row" vector of the old row's cells
   Update the index of every cell in the "r-th" row based on the format of the row above it */
void maze::initializeNextRow(int r)
{
	for (int i = 0; i < (int)row.size(); i++)
	{
		row[i].clear(); // clear all of the cells from the sets in the row above
	}

	for (int i = 0; i < (int)grid.size(); i++)
	{
		// update cells in the new row accordingly
		if (grid[r-1][i].getWall(DOWN))
		{
			grid[r][i].setEllerIndex(-1);
		}
		else
		{
			grid[r][i].setEllerIndex(grid[r-1][i].getEllerIndex());
			row[grid[r][i].getEllerIndex()].insert(grid[r][i].getCoords());
		}
	}
}

/* Clear the "row" vector of the second-to-last row's cells
   Change the index of every cell in the last row to that of the cell above it, and add it to its set */
void maze::initializeLastRow(int r)
{
  for (int i = 0; i < (int)row.size(); i++)
  {
    row[i].clear(); // clear all of the cells from the sets in the row above
  }

  for (int i = 0; i < (int)grid.size(); i++) // update every cell's index and add it to the corresponding set
  {
    grid[r][i].setEllerIndex(grid[r-1][i].getEllerIndex());
    row[grid[r-1][i].getEllerIndex()].insert(grid[r][i].getCoords());
  }
}

/* Use Eller's algorithm to finalize the last row */
void maze::implementLastRow(int r, SDL_Renderer* renderer)
{
  for (int i = 0; i < (int)grid.size() - 1; i++)
  {
    if (grid[r-1][i].getEllerIndex() != grid[r-1][i+1].getEllerIndex()) // if two cells are not currently in the same set
    {
      // union the sets
      set<pair<int, int>> set1 = row[grid[r][i].getEllerIndex()];
      set<pair<int, int>> set2 = row[grid[r][i+1].getEllerIndex()];
      set1.insert(set2.begin(), set2.end());

      row[grid[r][i+1].getEllerIndex()].clear(); // clear the set that got merged
      updateAllCellsInSet(set2, grid[r][i].getEllerIndex()); // update the index of all the cells that are being merged

      row[grid[r][i].getEllerIndex()] = set1;

      // remove the shared wall between them
      changeSharedWall(false, grid[r][i].getCoords(), grid[r][i+1].getCoords(), renderer);
    }
    else // if they are, add a wall between them given certain conditions
    {
      if ((grid[r-1][i].getWall(RIGHT) && !grid[r-1][i].getWall(DOWN)) || (i > 0 && grid[r-1][i].getWall(DOWN) && !grid[r][i-1].getWall(RIGHT)))
        changeSharedWall(true, grid[r][i].getCoords(), grid[r][i+1].getCoords(), renderer);
    }
  }
}

/* Draw lines around the border of the grid */
void maze::createBorder(SDL_Renderer* renderer)
{
	for (int i = 0; i < (int)grid.size(); i++)
	{
		if (i == 0 || i == (int)grid.size() - 1) // set and draw the top or bottom border
		{
			for (int j = 0; j < (int)grid.size(); j++)
			{
				grid[i][j].switchWall(i == 0 ? UP : DOWN, true);
				drawWall(i, j, 255, i == 0 ? UP : DOWN, renderer);
			}
		}

		grid[i][0].switchWall(LEFT, true); // set and draw the left border
		drawWall(i, 0, 255, LEFT, renderer);
		grid[i][grid.size() - 2].switchWall(RIGHT, true); // set and draw the right border
		drawWall(i, grid.size() - 1, 255, RIGHT, renderer);
	}
}

/* Draw all four walls for the cell at (r, c) */
void maze::drawBox(int r, int c, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(renderer, r*LINESIZE, c*LINESIZE, r*LINESIZE, c*LINESIZE + LINESIZE); // left
  SDL_RenderDrawLine(renderer, r*LINESIZE, c*LINESIZE, r*LINESIZE + LINESIZE, c*LINESIZE); // up
  SDL_RenderDrawLine(renderer, r*LINESIZE + LINESIZE, c*LINESIZE, r*LINESIZE + LINESIZE, c*LINESIZE + LINESIZE); // right
  SDL_RenderDrawLine(renderer, r*LINESIZE, c*LINESIZE + LINESIZE, r*LINESIZE + LINESIZE, c*LINESIZE + LINESIZE); // down
}

/* Draw a single wall for the cell at (r, c) */
void maze::drawWall(int c, int r, int color, int wall, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, color, color, color, SDL_ALPHA_OPAQUE);
  switch (wall) {
    case LEFT:
      SDL_RenderDrawLine(renderer, r*LINESIZE, c*LINESIZE, r*LINESIZE, c*LINESIZE + LINESIZE); // left
      break;
    case UP:
      SDL_RenderDrawLine(renderer, r*LINESIZE, c*LINESIZE, r*LINESIZE + LINESIZE, c*LINESIZE); // up
      break;
    case RIGHT:
      SDL_RenderDrawLine(renderer, r*LINESIZE + LINESIZE, c*LINESIZE, r*LINESIZE + LINESIZE, c*LINESIZE + LINESIZE); // right
      break;
    case DOWN:
      SDL_RenderDrawLine(renderer, r*LINESIZE, c*LINESIZE + LINESIZE, r*LINESIZE + LINESIZE, c*LINESIZE + LINESIZE); // down
      break;
  }
  
  SDL_RenderPresent(renderer);
  usleep(WAIT);
}
