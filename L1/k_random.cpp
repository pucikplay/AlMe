//
// Created by Gabriel on 18.03.2022.
//

#include "k_random.h"

typedef std::mt19937 MyRNG;
MyRNG rng1(time(nullptr));

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

void thread_shuffle(int** matrix, size_t n, size_t no_tasks, size_t thread_no, size_t *lengths, std::vector<int> *paths) {

    size_t length = SIZE_MAX;
    size_t best_length = SIZE_MAX;
    std::vector<int> best_permutation, permutation;

    for (size_t i = 0; i < n; i++) permutation.push_back(i);

    for (size_t i = 0; i < no_tasks; i++) {
        std::shuffle(std::begin(permutation), std::end(permutation), rng1);

        length = calculate_length(permutation, matrix, n);

        if (length < best_length) {
            best_length = length;
            best_permutation = permutation;
        }
    }

    lengths[thread_no] = best_length;
    paths[thread_no] = best_permutation;
}

std::vector<int> best_random_road_parallel(std::size_t k, std::size_t n, int** matrix, size_t no_threads) {

    std::vector<int> permutation, best_permutation, new_permutation(n);
    auto *lengths = new size_t[no_threads];
	auto *paths = new std::vector<int>[no_threads];
    size_t best_length = SIZE_MAX;
    std::thread th[no_threads];

    for (size_t i = 0; i < no_threads; i++) {
        paths[i] = new_permutation;
        th[i] = std::thread(thread_shuffle, matrix, n, size_t(k/no_threads), i, lengths, paths);
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
