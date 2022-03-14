//
// Created by Franek on 12.03.2022.
//

#ifndef ALME_ROADSOLVER_H
#define ALME_ROADSOLVER_H

#include <vector>

std::vector<int> firstRoadMaker(std::vector<std::pair<double, double>> coords, int** matrix);
std::vector<int> localEnhancer(std::vector<int> roadList, int** matrix, int start, int end);

#endif //ALME_ROADSOLVER_H
