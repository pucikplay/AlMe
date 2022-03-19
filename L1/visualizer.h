//
// Created by Franek on 11.03.2022.
//

#ifndef ALME_VISUALIZER_H
#define ALME_VISUALIZER_H

#include <vector>
#include "SDL.h"

SDL_Window* windowInitializing();

//Drawing "Basics"
void makePoints(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale);
void highlightPoints(SDL_Renderer *renderer, std::vector<int> localRoads, std::vector<std::pair<double, double>> coords, double fittedScale);
void makeEdges(SDL_Renderer *renderer, std::vector<int> roads, std::vector<std::pair<double, double>> coords, double fittedScale);

//Utils
std::vector<std::pair<double, double>> genCords(size_t n);
double computeScale(std::vector<std::pair<double, double>> coords);

//Drawing "Events"
void makeFirstPoints(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale);
void makeFirstRoad(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale, int** matrix, std::vector<int> roadList);
void makeLocalRoad(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale, int** matrix, std::vector<int> roadList, int k);
void make2OptSwap(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale, int** matrix, std::vector<int> roadList, std::pair<int, int> points);

//Drawing "Mains"
void drawNearestNeigh(std::vector<std::pair<double, double>> coords, int** matrix);
void drawKRandom(std::vector<std::pair<double, double>> coords, int** matrix, int k);
void draw2Opt(std::vector<std::pair<double, double>> coords, int** matrix);
void doSimpleVisual();

#endif //ALME_VISUALIZER_H
