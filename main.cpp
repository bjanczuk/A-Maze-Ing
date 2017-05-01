// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// main.cpp

#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <deque>
#include <math.h>

#include "cell.h"
#include "maze.h"

void SDLWaitForReturn();

enum OPTIONS {RECURSIVE = 1, PRIMS = 2, ELLERS = 4, KRUSKALS = 8}; // bitmask to distinguish the options

using namespace std;

void usage(const char *program_name) {
    cout << "Usage: " << program_name << " [ALGORITHMS] [OPTIONS]\n"
                    "\nAlgorithms: (if nothing is selected, the default is a random algorithm)\n"
                    "-recursive\tDraw a maze using recursive backtracking\n"
                    "-prims\t\tDraw a maze using Prim's algorithm\n"
                    "-kruskals\tDraw a maze using Kruskal's algorithm\n"
                    "-ellers\t\tDraw a maze using Eller's algorithm\n"
                    "-all\t\tDraw a maze using each of the 4 algorithms\n"
                    "Options:\n"
                    "-g (grid size)\tNumber of cells per row/column (default is 20)\n"
                    "-w (wait time)\tNumber of milliseconds to wait after each line gets drawn (default is 10000)\n";
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
      deque<int> options;
      char *arg;
      bool all = false;

      // set up default maze drawing values
      int grid_size = 20;
      int wait_time = 10000;
      const int cell_size = 30;

      while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') // parse command line arguments
      {
        arg = argv[argind++]; // store the flag
      
        // establish the options variable
        if (strcmp(arg, "-recursive") == 0)
          options.push_back(RECURSIVE);
        else if (strcmp(arg, "-prims") == 0)
          options.push_back(PRIMS);
        else if (strcmp(arg, "-ellers") == 0)
          options.push_back(ELLERS);
        else if (strcmp(arg, "-kruskals") == 0)
          options.push_back(KRUSKALS);
        else if (strcmp(arg, "-all") == 0)
          all = true;

        // change maze drawing variables
        else if (strcmp(arg, "-g") == 0)
          grid_size = stoi(argv[argind++]);
        else if (strcmp(arg, "-w") == 0)
          wait_time = stoi(argv[argind++]);
        else
          usage(argv[0]);
      }

      cout << "PRESS ENTER/RETURN TO ADVANCE THE WINDOW" << endl;

      // choose a random algorithm to show if there wasn't one selected
      if (options.size() == 0)
        options.push_back(pow(2, rand() % 4));

      // check which algorithm(s) got picked and draw each respective maze
      while (options.size())
      {
        if ((options.front() & RECURSIVE) == RECURSIVE || all)
        {
          SDL_CreateWindowAndRenderer(grid_size * cell_size + 2, grid_size * cell_size + 2, 0, &window, &renderer);
          SDL_SetWindowTitle(window, "Recursive Backtracking");

          maze newMaze(renderer, grid_size, wait_time); // make a new maze object
          newMaze.recursiveBack(newMaze.startCell(), renderer); // use the recursive backtracking algorithm
          SDLWaitForReturn(); // wait for the user to press enter before moving on
        }

        if ((options.front() & PRIMS) == PRIMS || all)
        {
          SDL_CreateWindowAndRenderer(grid_size * cell_size + 2, grid_size * cell_size + 2, 0, &window, &renderer);
          SDL_SetWindowTitle(window, "Prim's Algorithm");

          maze newMaze(renderer, grid_size, wait_time); // make a new maze object
          newMaze.prims(newMaze.startCell(), renderer); // use Prim's algorithm
          SDLWaitForReturn(); // wait for the user to press enter before moving on
        }

        if ((options.front() & KRUSKALS) == KRUSKALS || all)
        {
          SDL_CreateWindowAndRenderer(grid_size * cell_size + 2, grid_size * cell_size + 2, 0, &window, &renderer);
          SDL_SetWindowTitle(window, "Kruskal's Algorithm");

          maze newMaze(renderer, grid_size, wait_time); // make a new maze object
          newMaze.kruskals(renderer); // use Kruskal's algorithm
          SDLWaitForReturn(); // wait for the user to press enter before moving on
        }

        if ((options.front() & ELLERS) == ELLERS || all)
        {
          SDL_CreateWindowAndRenderer(grid_size * cell_size + 2, grid_size * cell_size + 2, 0, &window, &renderer);
          SDL_SetWindowTitle(window, "Eller's Algorithm");

          maze newMaze(renderer, grid_size, wait_time, false); // make a new maze object
          newMaze.ellers(renderer); // use Eller's algorithm
          SDLWaitForReturn(); // wait for the user to press enter before moving on
        }

        options.pop_front(); // remove each maze after it's been drawn
      }

      // once the mazes have been drawn, close the SDL renderer(s) and window(s)
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
    return 0;
}

void SDLWaitForReturn()
{
	SDL_Event event;
	while(true)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
			break;
	}
	return;
}
