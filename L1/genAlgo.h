//
// Created by Franek on 13.05.2022.
//

#ifndef ALME_GENALGO_H
#define ALME_GENALGO_H

#include <vector>

std::vector<int> geneticMain(size_t n, int** matrix, int populationSize = 20, double mutationThreshold = 0.05, int mutMode = 3, int mutationIntensification = 4, int iterations = 100, int crossMode = 0, int crossSize = 5, int crossType = 1, int crossCount = 21, int selectionMode = 1);
std::pair<std::vector<int>, int> geneticMainTimed(size_t n, int** matrix, int populationSize = 20, double mutationThreshold = 0.05, int mutMode = 3, int muttionIntensification = 4, double time = 60.0, int crossMode = 0, int crossSize = 5, int crossType = 1, int crossCount = 21, int selectionMode = 1, int startMode = 0);
std::vector<int> geneticIslands(size_t n, int** matrix, int populationSize, double mutationThreshold, int mutMode, int muttionIntensification, int wholeIterations, int crossMode, int crossSize, int crossType, int crossCount, int islandsNumber, int swappingInterval, int swapSize);

#endif //ALME_GENALGO_H
