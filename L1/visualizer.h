//
// Created by Franek on 11.03.2022.
//

#ifndef ALME_VISUALIZER_H
#define ALME_VISUALIZER_H

#include <vector>
#include "SDL.h"

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);
void doSimpleVisual();
void makePoints(SDL_Renderer *renderer, std::vector<std::pair<double, double>> coords, double fittedScale);
void makeEdges(SDL_Renderer *renderer, std::vector<std::pair<std::pair<double, double>, std::pair<double, double>>> roads, double fittedScale);
void doEucDraw(std::vector<std::pair<double, double>> coords, int** matrix);

#endif //ALME_VISUALIZER_H
