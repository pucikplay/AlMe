//
// Created by Gabriel on 18.03.2022.
//

#include <cstdint>
#include "2_opt.h"

size_t swap_length(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t n) {

	size_t newLength = length;

	int smaller = (road[i] < road[(i + 1) % n]) ? road[i] : road[(i + 1) % n];
	int bigger = (road[i] >= road[(i + 1) % n]) ? road[i] : road[(i + 1) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[j] < road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	bigger = (road[j] >= road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[i] < road[j]) ? road[i] : road[j];
	bigger = (road[i] >= road[j]) ? road[i] : road[j];
	newLength += matrix[smaller][bigger];

	smaller = (road[(i + 1) % n] < road[(j + 1) % n]) ? road[(i + 1) % n] : road[(j + 1) % n];
	bigger = (road[(i + 1) % n] >= road[(j + 1) % n]) ? road[(i + 1) % n] : road[(j + 1) % n];
	newLength += matrix[smaller][bigger];

	return newLength;
}

std::vector<int> swap_2_opt(std::vector<int> road, size_t i, size_t j) {
	std::reverse(road.begin() + i + 1, road.begin() + j + 1);
	return road;
}

std::vector<int> get_2_opt_road(std::vector<int> road, int** matrix, std::size_t n) {

	size_t best_length = calculate_length(road, matrix, n);
	size_t length = best_length;
	bool road_changed = true;
	bool jumpFlag = false;

	while (road_changed) {
		jumpFlag = false;
		road_changed = false;
		for (size_t i = 0; i < n - 1; i++) {
			for (size_t j = i + 2; j < n - 1; j++) {
				length = swap_length(best_length, road, matrix, i, j, n);
				if (length < best_length) {
					road = swap_2_opt(road, i, j);
					best_length = length;
					road_changed = true;
					jumpFlag = true;
				}
				if(jumpFlag)
					break;
			}
			if(jumpFlag)
				break;
		}
	}

	return road;
}

std::vector<std::pair<int, int>> get_2_opt_road_visual(std::vector<int> road, int** matrix, std::size_t n) {

	std::vector<std::pair<int, int>> changeList;
	size_t best_length = calculate_length(road, matrix, n);
	size_t length = best_length;
	bool road_changed = true;
	bool jumpFlag = false;

	while (road_changed) {
		jumpFlag = false;
		road_changed = false;
		for (size_t i = 0; i < n - 1; i++) {
			for (size_t j = i + 1; j < n; j++) {
				length = swap_length(best_length, road, matrix, i, j, n);
				if (length < best_length) {

					road = swap_2_opt(road, i, j);
					changeList.emplace_back(i, j);
					
					best_length = length;
					road_changed = true;
					jumpFlag = true;
				}
				if(jumpFlag)
					break;
			}
			if(jumpFlag)
				break;
		}
	}

	return changeList;
}