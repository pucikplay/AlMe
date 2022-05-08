//
// Created by Franek on 13.04.2022.
//

#ifndef ALME_TABU_SEARCH_H
#define ALME_TABU_SEARCH_H
#include <vector>
#include "2_opt.h"
#include "k_random.h"

// {roadVector, score}, {tabuList, tabuCounter}
typedef std::pair<std::pair<std::vector<int>, size_t>, std::pair<std::pair<size_t, size_t>*, int>> structLTM;

// Neighborhoods
size_t invertLength(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t n);
size_t insertLength(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t n);
size_t swapLength(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t n);

// Single move
std::vector<int> doStep(std::vector<int> road, size_t i, size_t j, int mode);

// Kicks
std::vector<int> parametrizedKick(std::vector<int> road, size_t n, std::pair<size_t, size_t> kickRange);
std::vector<int> deterministicKik(std::vector<int> road, int rseed, int mode, int size);

// Tabu dependencies
bool checkTabuList(std::pair<size_t, size_t> *tabuList, size_t n, size_t i, size_t j);
std::pair<size_t, std::pair<size_t, size_t>> checkNeighbourhood(std::vector<int> road, int** matrix, std::size_t n, size_t length, std::pair<size_t, size_t>* tabuList, int elementsOnTabu, int mode, size_t globalBestLen);

// Long-Term-Memory maintanance
std::vector<structLTM> addThingToLTM(std::vector<structLTM> returnList, structLTM newThing);
std::vector<structLTM> cutLast(std::vector<structLTM> returnList);

// Tabu Algorithms
std::vector<int> get_tabu_road(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode);
std::vector<int> deterministicTabu(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, int mode, int kikMode, int kikSize);
std::pair<std::vector<int>, int> deterministicTabuWithKikCount(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, int mode, int kikMode, int kikSize);
std::pair<std::vector<std::pair<int, int>>, std::vector<std::vector<int>>> get_tabu_road_visual(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode);

#endif //ALME_TABU_SEARCH_H
