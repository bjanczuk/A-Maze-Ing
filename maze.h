// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// maze.h

#ifndef MAZE_H
#define MAZE_H

#include "cell.h"

#include <iostream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

class maze {
  public:
    // Functions used for multiple algorithms
    maze(SDL_Renderer*, const int = 20, const int = 7500, bool draw = true);
    ~maze();
    pair<int, int> startCell();
    vector<pair<int, int>> getNeighbors(int, int);
    bool checkBoundaries(int, int);
    void changeSharedWall(bool, pair<int, int>, pair<int, int>, SDL_Renderer*);

    // Functions used for the recursive backtracking algorithm
    void recursiveBack(pair<int, int>, SDL_Renderer*);
    bool allVisited();
    pair<int, int> getRandomUnvisitedNeighbor(int, int);

    // Functions used for Prim's algorithm
    void prims(pair<int, int>, SDL_Renderer*);
    void addToFrontier(vector<pair<int, int>>);
    int  getRandomVisitedNeighbor(vector<pair<int, int>>);

    // Functions used for Kruskal's algorithm
    void kruskals(SDL_Renderer*);
    void addNeighborEdges(vector<pair<pair<int, int>, pair<int, int>>> &, int, int);

    // Functions used for Eller's algorithm
    void ellers(SDL_Renderer*);
    void updateUniqueSets(int);
    void randomizeRightWalls(int, SDL_Renderer*);
    void updateAllCellsInSet(set<pair<int, int>>, int);
    void randomizeBottomWalls(int, SDL_Renderer*);
    int  countBottomlessInSet(set<pair<int, int>>);
    void initializeNextRow(int);
    void initializeLastRow(int);
    void implementLastRow(int, SDL_Renderer*);

    // Functions used to draw the maze
    void createBorder(SDL_Renderer*);
    void drawBox(int, int, SDL_Renderer*);
    void drawWall(int, int, int, int, SDL_Renderer*);

  private:
    int size;
    vector<vector<cell>> grid;
    stack<cell> cellStack; // used for recursive backtracking
    set<pair<int, int>> frontier; // used for Prim's algorithm
    vector<set<pair<int, int>>> sets; // used for Kruskal's algorithm
    vector<set<pair<int, int>>> row; // used for Eller's algorithm
};

#endif


