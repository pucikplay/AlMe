//
// Created by Franek on 11.03.2022.
//

#include "SDL.h"
#include "visualizer.h"
#include <math.h>

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

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

void makePoints(std::vector<std::pair<double, double>> coords, int** matrix) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
		"Point Madness",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIN_WIDTH,
		WIN_HEIGHT,
		0
	);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Black screen
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 60, 120, 200, SDL_ALPHA_OPAQUE); // Cyan-like

	size_t n = coords.size();

	double maxX = 0, maxY = 0;
	for (int i = 0; i < n; i++) {

		if(coords[i].first > maxX)
			maxX = coords[i].first;

		if(coords[i].first > maxY)
			maxY = coords[i].second;
	}

	double scaleX = WIN_WIDTH / maxX;
	double scaleY = WIN_HEIGHT / maxY;

	double fittedScale = (scaleX < scaleY) ? scaleX : scaleY;

	SDL_RenderSetScale(renderer, fittedScale, fittedScale);
	for (int i = 0; i < n; i++) {
		SDL_RenderDrawPoint(renderer, (int)coords[i].first, (int)coords[i].second);
	}

	SDL_RenderPresent(renderer);
	SDL_Delay(5000);

	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}