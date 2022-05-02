//
// Created by Gabriel on 02.05.2022.
//

#include "tabu_multithreaded_wrapper.h"

void thread_tabu(std::vector<int> road, int** matrix, std::size_t n, size_t thread_no, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode, size_t *lengths, std::vector<int> *paths)
{
    paths[thread_no] = get_tabu_road(road, matrix, n, tabuSize, time, enhancementLimit, kickRange, mode);
    lengths[thread_no] = calculate_length(paths[thread_no], matrix, n);
}

std::vector<int> get_tabu_road_parallel(std::vector<std::pair<double, double>> coords, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode, size_t no_threads)
{
    std::vector<int> best_permutation;
    auto *lengths = new size_t[no_threads];
    auto *paths = new std::vector<int>[no_threads];
    size_t best_length = SIZE_MAX;
    std::thread th[no_threads];

    for (size_t i = 0; i < no_threads; i++) {
        std::vector<int> road = best_random_road(1000, coords.size(), matrix);
        paths[i] = road;
        th[i] = std::thread(thread_tabu, road, matrix, n, i, tabuSize, time, enhancementLimit, kickRange, mode, lengths, paths);
    }

    for (size_t i = 0; i < no_threads ; i++) {
        th[i].join();
    }

    for (size_t i = 0; i < no_threads ; i++) {
        if (lengths[i] < best_length) {
            best_length = lengths[i];
            best_permutation = paths[i];
        }
    }

    return best_permutation;
}
