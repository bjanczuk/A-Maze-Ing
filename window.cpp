#include <stdio.h>
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[])
{
	/*
	SDL_Window *window = NULL;
	SDL_Surface *screenSurface = NULL;
	bool quit = false;
	SDL_Event event;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		cout << SDL_GetError() << endl;
	window = SDL_CreateWindow("Maze Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	if (window == NULL)
		cout << "Window failed to open" << endl;

	screenSurface = SDL_GetWindowSurface(window);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	//SDL_RenderDrawLine(renderer, 10,10,10,10);


	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window);
	SDL_Delay(3000);
	*/

	if(SDL_Init(SDL_INIT_VIDEO) == 0){
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;

		if(SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0){
			SDL_bool done = SDL_FALSE;

			while(!done){
				SDL_Event event;
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				
				//format: SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
				SDL_RenderDrawLine(renderer, 200, 200, 200, 300);
				SDL_RenderDrawLine(renderer, 300, 200, 300, 300);
				SDL_RenderDrawLine(renderer, 200, 250, 300, 250);
				SDL_RenderDrawLine(renderer, 350, 250, 350, 300);
				SDL_RenderDrawLine(renderer, 350, 225, 350, 226);
				SDL_RenderPresent(renderer);

				while(SDL_PollEvent(&event)){
					if(event.type == SDL_QUIT){
						done = SDL_TRUE;
					}
				}
			}
		}
		
		if(renderer){
			SDL_DestroyRenderer(renderer);
		}
		if(window){
			SDL_DestroyWindow(window);
		}
	}
	SDL_Quit();
	return 0;
}
