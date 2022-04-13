//
// Created by Franek on 13.04.2022.
//

#include <cstdint>
#include "tabu_search.h"

typedef std::mt19937 MyRNG;
MyRNG rng2(time(nullptr));

std::vector<int> doParametrizedKick(std::vector<int> road, size_t n, size_t shuffleSize, size_t startingPoint) {

	std::vector<int> shuffledPart, result;

	for(int i = startingPoint; i < shuffleSize + startingPoint; i++)
		shuffledPart.emplace_back(road[i]);
	
	std::shuffle(std::begin(shuffledPart), std::end(shuffledPart), rng2);

	for(int i = 0; i < startingPoint; i++)
		result.emplace_back(road[i]);

	for(int i = 0; i < shuffleSize; i++)
		result.emplace_back(shuffledPart[i]);

	for(int i = shuffleSize + startingPoint; i < n; i++)
		result.emplace_back(road[i]);

	return result;
}

bool checkTabuList(std::pair<size_t, size_t> *tabuList, size_t tabuSize, size_t i, size_t j) {
	//printf("%ld - %ld\n", i, j);
	for(int k = 0; k < tabuSize; k++)
		if(tabuList[k].first == i && tabuList[k].second == j) {
			//printf("Tabu! ");
			return true;
		}
	return false;
}

//based on 2_opt
std::vector<int> get_tabu_road(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange) {

	size_t best_length = calculate_length(road, matrix, n);
	size_t length = best_length;
	bool road_changed = true;
	bool jumpFlag = false;

	size_t globalBestLen = best_length;
	std::vector<int> globalBestRoad = road;

	bool stop1 = true, stop2 = true;
	int lastEnhance = 0;

	std::pair<size_t, size_t> tabuList[tabuSize];
	for(int i = 0; i < tabuSize; i++)
		tabuList[i] = std::pair<size_t, size_t> {n + 1, n + 1};
	int tabuCounter = 0;

	auto start = std::chrono::high_resolution_clock::now();
	do {

		jumpFlag = false;
		road_changed = false;
		for (size_t i = 0; i < n - 1; i++) {
			for (size_t j = i + 2; j < n - 1; j++) {
				//TODO: add aspiration!!!
				if(checkTabuList(tabuList, tabuSize, i, j) == false) {
					//printf("%ld, %ld\n", i, j);
					length = swap_length(best_length, road, matrix, i, j, n);
					if (length < best_length) {
						road = swap_2_opt(road, i, j);
						best_length = length;
						road_changed = true;
						jumpFlag = true;

						tabuList[tabuCounter].first = i;
						tabuList[tabuCounter].second = j;
						tabuCounter += 1;
						tabuCounter %= tabuSize;

					}
					if(jumpFlag)
						break;
				}
			}
			if(jumpFlag)
				break;
		}

		if(!road_changed) {
			lastEnhance += 1;
			if(best_length < globalBestLen) {
				globalBestLen = best_length;
				globalBestRoad = road;
				printf("New local Minimum!!! : %ld\n", globalBestLen);
				lastEnhance = 0;
			}
			size_t shuffleRange = kickRange.second - kickRange.first;
			size_t shuffleSize = rng2() % shuffleRange + kickRange.first;
			size_t startPoint = rng2() % (n - kickRange.second - 1);
			road = doParametrizedKick(road, n, shuffleSize, startPoint);
			best_length = calculate_length(road, matrix, n);
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;
		stop2 = (lastEnhance < enhancementLimit) ? true : false;

		//printf("%d..%d\n", stop1, stop2);

	} while (stop1 && stop2);

	return globalBestRoad;
}

std::vector<std::pair<int, int>> get_tabu_road_visual(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange) {

	std::vector<std::pair<int, int>> changeList;
	size_t best_length = calculate_length(road, matrix, n);
	size_t length = best_length;
	bool road_changed = true;
	bool jumpFlag = false;

	size_t globalBestLen = best_length;
	std::vector<int> globalBestRoad = road;

	bool stop1 = true, stop2 = true, stop3;
	int lastEnhance = 0;

	std::pair<size_t, size_t> tabuList[tabuSize];
	for(int i = 0; i < tabuSize; i++)
		tabuList[i] = std::pair<size_t, size_t> {n + 1, n + 1};
	int tabuCounter = 0;

	auto start = std::chrono::high_resolution_clock::now();
	do {

		jumpFlag = false;
		road_changed = false;
		for (size_t i = 0; i < n - 1; i++) {
			for (size_t j = i + 2; j < n - 1; j++) {
				//TODO: add aspiration!!!
				if(checkTabuList(tabuList, tabuSize, i, j) == false) {
					//printf("%ld, %ld\n", i, j);
					length = swap_length(best_length, road, matrix, i, j, n);
					if (length < best_length) {
						road = swap_2_opt(road, i, j);
						best_length = length;
						road_changed = true;
						jumpFlag = true;

						changeList.emplace_back(i, j);

						tabuList[tabuCounter].first = i;
						tabuList[tabuCounter].second = j;
						tabuCounter += 1;
						tabuCounter %= tabuSize;

					}
					if(jumpFlag)
						break;
				}
			}
			if(jumpFlag)
				break;
		}

		if(!road_changed) {
			lastEnhance += 1;
			if(best_length < globalBestLen) {

				//printf("\nOld:\n");
				//for(int i = 0; i < n; i++)
				//	printf("%d -> ", globalBestRoad[i]);
				
				globalBestLen = best_length;
				globalBestRoad = road;
				printf("New local Minimum!!! : %ld\n", globalBestLen);
				lastEnhance = 0;

				//printf("\nNew:\n");
				//for(int i = 0; i < n; i++)
				//	printf("%d -> ", globalBestRoad[i]);

				//printf("\n");
			}
			size_t shuffleRange = kickRange.second - kickRange.first;
			size_t shuffleSize = rng2() % shuffleRange + kickRange.first;
			size_t startPoint = rng2() % (n - kickRange.second - 1);
			road = doParametrizedKick(road, n, shuffleSize, startPoint);
			best_length = calculate_length(road, matrix, n);
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;
		stop2 = (lastEnhance < enhancementLimit) ? true : false;

		//printf("%d..%d\n", stop1, stop2);
		//printf("%ld\n", (std::chrono::high_resolution_clock::now() - start).count());

	} while (stop1 && stop2);

	return changeList;
}