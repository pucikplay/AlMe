//
// Created by Franek on 13.05.2022.
//

#ifndef ALME_GENALGO_H
#define ALME_GENALGO_H

#include <vector>

std::vector<int> geneticMain(size_t n, int** matrix, int populationSize = 20, double mutationThreshold = 0.05, int mutMode = 3, int mutationIntensification = 4, int iterations = 100, int crossMode = 0, int crossSize = 5);

#endif //ALME_GENALGO_H
