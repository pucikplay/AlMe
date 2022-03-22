//
// Created by Franek on 12.03.2022.
//

#ifndef ALME_ROADSOLVER_H
#define ALME_ROADSOLVER_H

#include <vector>

std::vector<int> bestFullBranchingNeighbor(size_t n, int** matrix);
std::vector<int> startFullBranchingNeighbor(size_t n, int** matrix, int k);
std::vector<int> getFullNearestBranch(size_t n, int** matrix, std::vector<int> available, int k);

std::vector<int> bestBranchingNeighbor(size_t n, int** matrix, int maxDepth);
size_t doBranching(std::vector<int> roadList, size_t n, int point, int** matrix, int depthLeft);
std::vector<int> doBranchingNeighbor(size_t n, int** matrix, int k, int maxDepth);

std::vector<int> bestStartingNeighbor(size_t n, int** matrix);
std::vector<int> doNearestNeighbor(std::size_t coords, int** matrix, int k);
std::vector<int> localEnhancer(std::vector<int> roadList, int** matrix, int start, int end);

#endif //ALME_ROADSOLVER_H
