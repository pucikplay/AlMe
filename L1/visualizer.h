//
// Created by Franek on 11.03.2022.
//

#ifndef ALME_VISUALIZER_H
#define ALME_VISUALIZER_H

#include <vector>
#include "SDL.h"

void doSimpleVisual();
void makePoints(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale);
void highlightPoints(SDL_Renderer *renderer, std::vector<int> localRoads, std::vector<std::pair<double, double>> coords, double fittedScale);
void makeEdges(SDL_Renderer *renderer, std::vector<int> roads, std::vector<std::pair<double, double>> coords, double fittedScale);
void doEucDraw(std::vector<std::pair<double, double>> coords, int** matrix);

#endif //ALME_VISUALIZER_H
