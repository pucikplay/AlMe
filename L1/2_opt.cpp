//
// Created by Gabriel on 18.03.2022.
//

#include <cstdint>
#include "2_opt.h"

size_t swap_length(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t n) {

    length -= matrix[road[i]][road[i+1]];
    length -= matrix[road[j]][road[(j+1)%n]];
    length += matrix[road[i]][road[(j+1)%n]];
    length += matrix[road[j]][road[i+1]];

    return length;
}

std::vector<int> swap_2_opt(std::vector<int> road, size_t i, size_t j) {
    std::reverse(road.begin() + i, road.begin() + j);
    return road;
}

std::vector<int> get_2_opt_road(std::vector<int> road, int** matrix, std::size_t n) {

    std::vector<int> new_road;
    size_t best_length = calculate_length(road, matrix, n);
    size_t length = best_length;
    bool road_changed = true;

    while (road_changed) {
        road_changed = false;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = i + 1; j < n; j++) {
                length = swap_length(length, road, matrix, i, j, n);
                if (length < best_length) {
                    road = swap_2_opt(road, i, j);
                    best_length = length;
                    road_changed = true;
                }
            }
        }
    }

    return road;
}
