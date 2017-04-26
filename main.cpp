// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// main.cpp

#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include "cell.h"
#include "maze.h"

enum OPTIONS {RECURSIVE = 1, PRIMS = 2, ELLERS = 4, KRUSKALS = 8}; // bitmask to decide the options

using namespace std;

void usage(const char *program_name) {
    cout << "Usage: " << program_name << " [-recursive] [-prims] [-ellers] [-kruskals] [-g grid_size]"
                    "[-w wait_time (in milliseconds)]\n" << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    srand(time(NULL)); // seed the random number generator
    if (SDL_Init(SDL_INIT_VIDEO) == 0) 
    {
      SDL_Window* window = NULL;
      SDL_Renderer* renderer = NULL;

      int argind = 1;
      int options = 0;
      char *arg;

      int grid_size = 20;
      int wait_time = 10000;
      int cell_size = 30;

      bool algorithm = false; // indicates whether a flag for an algorithm was used or not

      while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') // parse command line arguments
      {
        arg = argv[argind++]; // store the flag
      
        // establish the options variable
        if (strcmp(arg, "-recursive") == 0)
        {
          options |= RECURSIVE;
          algorithm = true;
        }
        else if (strcmp(arg, "-prims") == 0)
        {
          options |= PRIMS;
          algorithm = true;
        }
        else if (strcmp(arg, "-ellers") == 0)
        {
          options |= ELLERS;
          algorithm = true;
        }
        else if (strcmp(arg, "-kruskals") == 0)
        {
          options |= KRUSKALS;
          algorithm = true;
        }
        else if (strcmp(arg, "-g") == 0)
          grid_size = stoi(argv[argind++]);
        else if (strcmp(arg, "-w") == 0)
          wait_time = stoi(argv[argind++]);
        else
          usage(argv[0]);
      }

      if (algorithm == false)
        usage(argv[0]);

      if (SDL_CreateWindowAndRenderer(grid_size * cell_size, grid_size * cell_size, 0, &window, &renderer) == 0) // SDL functions are working
      {
        if ((options & RECURSIVE) == RECURSIVE)
        {
          SDL_SetWindowTitle(widnow, "Recursive Backtracking");
          maze newMaze(renderer, grid_size, wait_time);
          newMaze.recursiveBack(newMaze.startCell(), renderer);
          cin.ignore();
        }

        if ((options & PRIMS) == PRIMS)
        {
          SDL_SetWindowTitle(widnow, "Prim's Algorithm");
          maze newMaze(renderer, grid_size, wait_time);
          newMaze.prims(newMaze.startCell(), renderer);
          cin.ignore();
        }

        if ((options & ELLERS) == ELLERS)
        {
          SDL_SetWindowTitle(widnow, "Eller's Algorithm");
          maze newMaze(renderer, grid_size, wait_time);
          //newMaze.ellers(newMaze.startCell(), renderer);
          cin.ignore();
        }

        if ((options & KRUSKALS) == KRUSKALS)
        {
          SDL_SetWindowTitle(widnow, "Kruskal's Algorithm");
          maze newMaze(renderer, grid_size, wait_time);
          //newMaze.kruskals(newMaze.startCell(), renderer);
          cin.ignore();
        }
          
        if (renderer)
        {
          SDL_DestroyRenderer(renderer);
        }
        if (window)
        {
          SDL_DestroyWindow(window);
        }
        SDL_Quit();
      }
    }
    return 0;
}
