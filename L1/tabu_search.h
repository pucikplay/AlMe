//
// Created by Franek on 13.04.2022.
//

#ifndef ALME_TABU_SEARCH_H
#define ALME_TABU_SEARCH_H
#include <vector>
#include "2_opt.h"
#include "k_random.h"

std::vector<int> doParametrizedKick(std::vector<int> road, size_t n, size_t shuffleSize, size_t startingPoint);
bool checkTabuList(std::pair<size_t, size_t> *tabuList, size_t n, size_t i, size_t j);
std::vector<int> get_tabu_road(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange);
std::vector<std::pair<int, int>> get_tabu_road_visual(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange);

#endif //ALME_TABU_SEARCH_H
