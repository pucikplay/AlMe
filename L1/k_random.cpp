//
// Created by Gabriel on 18.03.2022.
//

#include "k_random.h"
#include <chrono>
#include <omp.h>

typedef std::mt19937 MyRNG;
MyRNG rng1;

size_t calculate_length(const std::vector<int>& permutation, int** matrix, size_t n) {
	size_t length = 0;
	//printf("New road\n");
	for (size_t i = 0; i < n; i++) {
		int smaller = (permutation[i] < permutation[(i + 1) % n]) ? permutation[i] : permutation[(i + 1) % n];
		int bigger = (permutation[i] >= permutation[(i + 1) % n]) ? permutation[i] : permutation[(i + 1) % n];
		length += matrix[smaller][bigger];
		//printf("%ld - %d\n", length, matrix[smaller][bigger]);
	}
	return length;
}

std::vector<int> best_random_road(std::size_t k, std::size_t n, int** matrix) {

	std::vector<int> permutation;
	std::vector<int> best_permutation;
	size_t best_length = SIZE_MAX;
	size_t length = 0;

	for (size_t i = 0; i < n; i++) permutation.push_back(i);

	for (size_t i = 0; i < k; i++) {
		std::shuffle(std::begin(permutation), std::end(permutation), rng1);

		length = calculate_length(permutation, matrix, n);

		if (length < best_length) {
			best_length = length;
			best_permutation = permutation;
			//printf("%ld\n", best_length);
		}
	}

	return best_permutation;
}

std::vector<int> best_random_road_timed(std::size_t n, int** matrix, double time) {

    std::vector<int> permutation;
    std::vector<int> best_permutation;
    size_t best_length = SIZE_MAX;
    size_t length = 0;

    for (size_t i = 0; i < n; i++) permutation.push_back(i);

    auto start = std::chrono::high_resolution_clock::now();
    do {
        std::shuffle(std::begin(permutation), std::end(permutation), rng1);

        length = calculate_length(permutation, matrix, n);

        if (length < best_length) {
            best_length = length;
            best_permutation = permutation;
            //printf("%ld\n", best_length);
        }
    } while ((std::chrono::high_resolution_clock::now() - start).count() < time);

    return best_permutation;
}

std::vector<int> best_random_road_parallel(std::size_t k, std::size_t n, int** matrix) {

    std::vector<int> permutation, best_permutation, new_permutation(n);
    size_t lengths[k];
	std::vector<int> paths[k];
    size_t best_length = SIZE_MAX;
    size_t length = 0;

    for (size_t i = 0; i < n; i++) permutation.push_back(i);

    omp_set_dynamic(1);
    #pragma omp parallel for num_threads(6)
        for (size_t i = 0; i < k; i++) {
            for (size_t j = 0; j < n; j++) {
				new_permutation[j] = permutation[j];
			}
            std::shuffle(std::begin(new_permutation), std::end(new_permutation), rng1);

            length = calculate_length(new_permutation, matrix, n);

            lengths[i] = length;
			paths[i] = new_permutation;
        }

    for (size_t i = 0; i < k ; i++) {
        if (lengths[i] < best_length) {
            best_length = lengths[i];
            best_permutation = paths[i];
        }
    }

//	for (size_t i = 0; i < n; i++) {
//		printf("%d ", best_permutation[i]);
//	}

    return best_permutation;
}
