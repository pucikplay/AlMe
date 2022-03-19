//
// Created by Gabriel on 18.03.2022.
//

#ifndef ALME_2_OPT_H
#define ALME_2_OPT_H
#include <vector>
#include "k_random.h"

size_t swap_length(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t n);
std::vector<int> swap_2_opt(std::vector<int> road, size_t i, size_t j);
std::vector<int> get_2_opt_road(std::vector<int> initial, int** matrix, std::size_t n);
std::vector<std::pair<int, int>> get_2_opt_road_visual(std::vector<int> road, int** matrix, std::size_t n);

#endif //ALME_2_OPT_H
