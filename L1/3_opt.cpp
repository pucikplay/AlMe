//
// Created by Franek on 23.03.2022.
//

#include <cstdint>
#include "3_opt.h"
#include "2_opt.h"

size_t swap_triple_length(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t k, size_t n) {

	size_t newLength = length;

	int smaller = (road[i] < road[(i + 1) % n]) ? road[i] : road[(i + 1) % n];
	int bigger = (road[i] >= road[(i + 1) % n]) ? road[i] : road[(i + 1) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[j] < road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	bigger = (road[j] >= road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[k] < road[(k + 1) % n]) ? road[k] : road[(k + 1) % n];
	bigger = (road[k] >= road[(k + 1) % n]) ? road[k] : road[(k + 1) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[i] < road[j]) ? road[i] : road[j];
	bigger = (road[i] >= road[j]) ? road[i] : road[j];
	newLength += matrix[smaller][bigger];

	smaller = (road[(i + 1) % n] < road[k]) ? road[(i + 1) % n] : road[k];
	bigger = (road[(i + 1) % n] >= road[k]) ? road[(i + 1) % n] : road[k];
	newLength += matrix[smaller][bigger];

	smaller = (road[(j + 1) % n] < road[(k + 1) % n]) ? road[(j + 1) % n] : road[(k + 1) % n];
	bigger = (road[(j + 1) % n] >= road[(k + 1) % n]) ? road[(j + 1) % n] : road[(k + 1) % n];
	newLength += matrix[smaller][bigger];

	return newLength;
}

std::pair<size_t, int> swap_every_length(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t k, size_t n) {
	size_t bestRoad = SIZE_MAX;
	int bestCase = -1;
	size_t tmpLength;

	//case 0
	tmpLength = swap_length(length, road, matrix, i, j, n);
	if(tmpLength < bestRoad) {
		bestRoad = tmpLength;
		bestCase = 0;
	}

	//case 1
	tmpLength = swap_length(length, road, matrix, j, k, n);
	if(tmpLength < bestRoad) {
		bestRoad = tmpLength;
		bestCase = 1;
	}

	//case 2
	tmpLength = swap_length(length, road, matrix, k, i, n);
	if(tmpLength < bestRoad) {
		bestRoad = tmpLength;
		bestCase = 2;
	}

	//case 3
	tmpLength = swap_triple_length(length, road, matrix, i, j, k, n);
	if(tmpLength < bestRoad) {
		bestRoad = tmpLength;
		bestCase = 3;
	}

	//case 4
	tmpLength = swap_triple_length(length, road, matrix, j, k, i, n);
	if(tmpLength < bestRoad) {
		bestRoad = tmpLength;
		bestCase = 4;
	}

	//case 5
	tmpLength = swap_triple_length(length, road, matrix, k, i, j, n);
	if(tmpLength < bestRoad) {
		bestRoad = tmpLength;
		bestCase = 5;
	}

	std::pair<size_t, int> resultPair;
	resultPair.first = bestRoad;
	resultPair.second = bestCase;

	return resultPair;
}

std::vector<int> swap_3_opt(std::vector<int> road, size_t i, size_t j, size_t k, int caseNum) {
	if(caseNum == 0)
		std::reverse(road.begin() + i + 1, road.begin() + j + 1);
	else if(caseNum == 1)
		std::reverse(road.begin() + j + 1, road.begin() + k + 1);
	else if(caseNum == 2)
		std::reverse(road.begin() + i + 1, road.begin() + k + 1);
	else if(caseNum == 3) {
		std::reverse(road.begin() + i + 1, road.begin() + j + 1);
		std::reverse(road.begin() + j + 1, road.begin() + k + 1);
	}
	else if(caseNum == 4) {
		std::reverse(road.begin() + j + 1, road.begin() + k + 1);
		std::reverse(road.begin() + i + 1, road.begin() + k + 1);
	}
	else if(caseNum == 5) {
		std::reverse(road.begin() + i + 1, road.begin() + j + 1);
		std::reverse(road.begin() + i + 1, road.begin() + k + 1);
	}
	return road;
}

std::vector<int> get_3_opt_road(std::vector<int> road, int** matrix, std::size_t n) {
	size_t best_length = calculate_length(road, matrix, n);
	size_t length = best_length;
	bool road_changed = true;
	bool jumpFlag = false;

	std::pair<size_t, int> resultPair;
	int caseNum;

	while (road_changed) {
		jumpFlag = false;
		road_changed = false;
		for (size_t i = 0; i < n - 2; i++) {
			for (size_t j = i + 2; j < n - 2; j++) {
				for (size_t k = j + 2; k < n - 2; k++) {
					resultPair = swap_every_length(best_length, road, matrix, i, j, k, n);
					length = resultPair.first;
					caseNum = resultPair.second;
					if (length < best_length) {
						road = swap_3_opt(road, i, j, k, caseNum);
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
			if(jumpFlag)
				break;
		}
	}

	return road;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> get_3_opt_road_visual(std::vector<int> road, int** matrix, std::size_t n) {
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> changeList;
	size_t best_length = calculate_length(road, matrix, n);
	size_t length = best_length;
	bool road_changed = true;
	bool jumpFlag = false;

	std::pair<size_t, int> resultPair;
	std::pair<int, int> firstPart, secondPart;
	int caseNum;

	while (road_changed) {
		jumpFlag = false;
		road_changed = false;
		for (size_t i = 0; i < n - 2; i++) {
			for (size_t j = i + 2; j < n - 2; j++) {
				for (size_t k = j + 2; k < n - 2; k++) {
					resultPair = swap_every_length(best_length, road, matrix, i, j, k, n);
					length = resultPair.first;
					caseNum = resultPair.second;
					if (length < best_length) {
						road = swap_3_opt(road, i, j, k, caseNum);

						firstPart.first = i;
						firstPart.second = j;
						secondPart.first = k;
						secondPart.second = caseNum;
						changeList.emplace_back(firstPart, secondPart);

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
			if(jumpFlag)
				break;
		}
	}

	return changeList;
}