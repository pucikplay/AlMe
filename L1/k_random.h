//
// Created by Gabriel on 18.03.2022.
//

#ifndef ALME_K_RANDOM_H
#define ALME_K_RANDOM_H
#include <vector>
#include <algorithm>
#include <random>

std::vector<int> best_random_road(size_t k, size_t n, int** matrix);
size_t calculate_length(const std::vector<int>& permutation, int** matrix, size_t n);
std::vector<int> best_random_road_parallel(std::size_t k, std::size_t n, int** matrix);

#endif //ALME_K_RANDOM_H
