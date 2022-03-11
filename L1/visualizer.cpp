#include "SDL.h"
#include "visualizer.h"

void doSimpleVisual() {

    SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
		"3-Second Triangle",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		0
	);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Black screen
	SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 60, 120, 200, SDL_ALPHA_OPAQUE); // Cyan-like

    //SDL_RenderSetScale( renderer, 2.0, 2.0);
    SDL_RenderDrawLine(renderer, 200, 300, 440, 300);

    //SDL_RenderSetScale( renderer, 4.0, 4.0);
    SDL_RenderDrawLine(renderer, 440, 300, 320, 100);

    //SDL_RenderSetScale( renderer, 8.0, 8.0);
    SDL_RenderDrawLine(renderer, 320, 100, 200, 300);

    SDL_RenderPresent(renderer);

	SDL_Delay(3000);

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
    	SDL_DestroyWindow(window);
    }
	SDL_Quit();
}