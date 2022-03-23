//
// Created by Franek on 23.03.2022.
//

#ifndef ALME_3_OPT_H
#define ALME_3_OPT_H
#include <vector>
#include "k_random.h"

size_t swap_triple_length(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t k, size_t n);
std::pair<size_t, int> swap_every_length(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t k, size_t n);
std::vector<int> swap_3_opt(std::vector<int> road, size_t i, size_t j, size_t k, int caseNum) ;
std::vector<int> get_3_opt_road(std::vector<int> road, int** matrix, std::size_t n);
std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> get_3_opt_road_visual(std::vector<int> road, int** matrix, std::size_t n);

#endif //ALME_3_OPT_H
