// Jessica Hardey, Bart Janczuk, Josefa Osorio, Aidan Lewis
// main.cpp

#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>

#include "cell.h"
#include "maze.h"

using namespace std;

int main(int argc, char* argv[])
{
  srand(time(NULL));
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (SDL_CreateWindowAndRenderer(610, 610, 0, &window, &renderer) == 0) {
      SDL_bool done = SDL_FALSE;
      while (!done) {
	SDL_Event event;
	maze newMaze(renderer, 5);
	//newMaze.recursiveBack(make_pair(0, 0), renderer);
	newMaze.prims(make_pair(0, 0), renderer);

	while (SDL_PollEvent(&event)) {
	  if (event.type == SDL_QUIT) {
	    done = SDL_TRUE;
	  }
	}
      }
    }
    if (renderer) {
      SDL_DestroyRenderer(renderer);
    }
    if (window) {
      SDL_DestroyWindow(window);
    }
  }
  SDL_Quit();
  return 0;
}
