//
// Created by Gabriel on 02.05.2022.
//

#ifndef ALME_TABU_MULTITHREADED_WRAPPER_H
#define ALME_TABU_MULTITHREADED_WRAPPER_H

#include <vector>
#include <cstddef>
#include <thread>
#include "tabu_search.h"
#include "k_random.h"

std::vector<int> get_tabu_road_parallel(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode, size_t no_threads);

#endif //ALME_TABU_MULTITHREADED_WRAPPER_H
