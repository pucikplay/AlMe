//
// Created by Franek on 11.03.2022.
//

#include "SDL.h"
#include "visualizer.h"
#include "roadSolver.h"
#include <math.h>
#include <stdio.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 960

SDL_Window* windowInitializing() {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
		"Point Madness",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIN_WIDTH,
		WIN_HEIGHT,
		0
	);

	return window;
}

//Drawing "Basics"
void makePoints(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale) {

	SDL_SetRenderDrawColor(renderer, 60, 120, 200, 255); // Cyan-like
	for (int i = 0; i < coords.size(); i++) {
		SDL_Rect pointRect;
		pointRect.x = (int)coords[i].first * (int)fittedScale;
		pointRect.y = (int)coords[i].second * (int)fittedScale;
		pointRect.w = (int)fittedScale;
		pointRect.h = (int)fittedScale;

		SDL_RenderDrawRect(renderer, &pointRect);
		SDL_RenderFillRect(renderer, &pointRect);
	}
}

void highlightPoints(SDL_Renderer *renderer, std::vector<int> roadPoints, std::vector<std::pair<double, double>> coords, double fittedScale) {

	SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255); // Green-like
	for (int i = 0; i < roadPoints.size(); i++) {
		SDL_Rect pointRect;
		pointRect.x = (int)coords[roadPoints[i]].first * (int)fittedScale - (int)fittedScale;
		pointRect.y = (int)coords[roadPoints[i]].second * (int)fittedScale - (int)fittedScale;
		pointRect.w = 2 * (int)fittedScale;
		pointRect.h = 2 * (int)fittedScale;

		SDL_RenderDrawRect(renderer, &pointRect);
		SDL_RenderFillRect(renderer, &pointRect);
	}
}

void makeEdges(SDL_Renderer *renderer, std::vector<int> roads, std::vector<std::pair<double, double>> coords, double fittedScale) {
	
	size_t n = coords.size();
	SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Orange-like
	for(int i = 0; i < n; i++) {
		int x1 = (int)coords[roads[i]].first * (int)fittedScale + (int)(fittedScale / 2);
		int y1 = (int)coords[roads[i]].second * (int)fittedScale + (int)(fittedScale / 2);
		int x2 = (int)coords[roads[(i + 1)% n]].first * (int)fittedScale + (int)(fittedScale / 2);
		int y2 = (int)coords[roads[(i + 1)% n]].second * (int)fittedScale + (int)(fittedScale / 2);
		//printf("%d %d - %d %d\n", x1, y1, x2, y2);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}
}

//Utils
std::vector<std::pair<double, double>> genCords(size_t n) {

	std::vector<std::pair<double, double>> coords;
	double centerX = WIN_WIDTH / 8.0 + 10.0;
	double centerY = WIN_HEIGHT / 8.0 + 10.0;
	double radius = (centerX < centerY) ? centerX - 10.0 : centerY - 10.0;

	for (int i = 0; i < n; i++) {
		double coordX = centerX - radius * sin(2 * M_PI * (double(i) / double(n)));
		double coordY = centerY - radius * cos(2 * M_PI * (double(i) / double(n)));
		coordX = ceil(coordX);
		coordY = ceil(coordY);
		coords.emplace_back(coordX, coordY);
	}

	return coords;
}

double computeScale(std::vector<std::pair<double, double>> coords) {
	
	int n = coords.size();
	double maxX = 0, maxY = 0;

	for (int i = 0; i < n; i++) {

		if(coords[i].first > maxX)
			maxX = coords[i].first;

		if(coords[i].second > maxY)
			maxY = coords[i].second;
	}

	double scaleX = WIN_WIDTH / maxX;
	double scaleY = WIN_HEIGHT / maxY;
	double fittedScale = (scaleX < scaleY) ? scaleX : scaleY;

	return fittedScale;
}

//Drawing "Events"
void makeFirstPoints(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	makePoints(renderer, coords, fittedScale);
	SDL_RenderPresent(renderer);

	SDL_Delay(1000);
}

std::vector<int> makeFirstRoad(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale, int** matrix) {
	std::vector<int> roadList = firstRoadMaker(coords.size(), matrix);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Black screen
	SDL_RenderClear(renderer);

	makeEdges(renderer, roadList, coords, fittedScale);
	makePoints(renderer, coords, fittedScale);
	SDL_RenderPresent(renderer);

	SDL_Delay(2000);
	return roadList;
}

std::vector<int> makeLocalRoad(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale, int** matrix, std::vector<int> roadList, int k) {
	std::vector<int> newRoadList = localEnhancer(roadList, matrix, k, k + 4);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Black screen
	SDL_RenderClear(renderer);

	makeEdges(renderer, newRoadList, coords, fittedScale);
	makePoints(renderer, coords, fittedScale);

	std::vector<int> roadPoints;
	for(int j = 0; j <= 4; j++)
		roadPoints.emplace_back(newRoadList[k + j]);

	highlightPoints(renderer, roadPoints, coords, fittedScale);
	SDL_RenderPresent(renderer);
	
	return newRoadList;
}

//Drawing "Mains"
void doDrawing(std::vector<std::pair<double, double>> coords, int** matrix) {

	SDL_Window* window = windowInitializing();
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	std::vector<int> roadList;
	double fittedScale = computeScale(coords);
	int delayTime = (20000 / coords.size() < 200) ? 20000 / coords.size() : 150;

	//Animation Loop logic
	bool quit = false;
	int actionCounter = 0;
	SDL_Event e;

	while(!quit) {

		while(SDL_PollEvent(&e) != 0)
			if(e.type == SDL_QUIT)
				quit = true;

		if(actionCounter == 0)
			makeFirstPoints(renderer, coords, fittedScale);
		else if(actionCounter == 1)
			roadList = makeFirstRoad(renderer, coords, fittedScale, matrix);
		else if(actionCounter < coords.size() - 2) {
			roadList = makeLocalRoad(renderer, coords, fittedScale, matrix, roadList, actionCounter - 2);
			SDL_Delay(delayTime);
		}
		else if(actionCounter == coords.size())
			SDL_Delay(2000);
		else
			quit = true;

		actionCounter += 1;
	}

	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}

void doSimpleVisual() {

	SDL_Window* window = windowInitializing();

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