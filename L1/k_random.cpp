//
// Created by Gabriel on 18.03.2022.
//

#include "k_random.h"

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

std::vector<int> best_random_road_parallel(std::size_t k, std::size_t n, int** matrix) {

    std::vector<int> permutation;
    std::vector<int> best_permutation;
    size_t best_length = SIZE_MAX;
    size_t length = 0;

    for (size_t i = 0; i < n; i++) permutation.push_back(i);

    #pragma omp parallel for num_threads(k)
        for (size_t i = 0; i < k; i++) {
            std::shuffle(std::begin(permutation), std::end(permutation), rng1);

            length = calculate_length(permutation, matrix, n);

            #pragma omp critical
                if (length < best_length) {
                    best_length = length;
                    best_permutation = permutation;
                    //printf("%ld\n", best_length);
                }
        }

    return best_permutation;
}
