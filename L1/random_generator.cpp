//
// Created by Gabriel on 13.03.2022.
//

#include "random_generator.h"

typedef std::mt19937 MyRNG;
MyRNG rng;

int get_random(int range) {
    std::uniform_int_distribution<uint32_t> new_dist(1,range);
    return new_dist(rng);
}

bool exists(std::vector<std::pair<double, double>> list, size_t x, size_t y) {
    for (size_t i = 0; i < list.size(); i++) {
        if (x == list[i].first && y == list[i].second) return true;
    }
    return false;
}

std::vector<std::pair<double, double>> gen_coords(std::size_t n) {
    rng.seed(time(nullptr));
    std::vector<std::pair<double, double>> list;
    size_t range = 2 * n;
    size_t x, y;

    for (size_t i = 0; i < n; i++) {
        x = get_random(range);
        y = get_random(range);

        while (exists(list, x, y)) {
            x = get_random(range);
            y = get_random(range);
        }

        list.emplace_back(x, y);
    }

    return list;
}