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
	size_t length = best_length, computed_length;
	bool road_changed = true;

	size_t globalBestLen = best_length;
	std::vector<int> globalBestRoad = road;

	bool stop1 = true, kikCond = true;
	int lastEnhance = 0;

	std::pair<size_t, size_t> bestPair;
	std::pair<size_t, size_t> tabuList[tabuSize];
	for(int i = 0; i < tabuSize; i++)
		tabuList[i] = std::pair<size_t, size_t> {n + 1, n + 1};
	int tabuCounter = 0, elementsOnTabu = 0;

	auto start = std::chrono::high_resolution_clock::now();
	do {

		road_changed = false;
		for (size_t i = 0; i < n - 1; i++) {
			for (size_t j = i + 2; j < n - 1; j++) {
				//TODO: add aspiration!!!
				if(checkTabuList(tabuList, elementsOnTabu, i, j) == false) {
					//printf("%ld, %ld\n", i, j);
					computed_length = swap_length(length, road, matrix, i, j, n);
					if (computed_length < best_length) {
						//printf("b - %ld ", computed_length);
						bestPair = {i, j};
						best_length = computed_length;
						road_changed = true;
					}
				}
			}
		}

		tabuList[tabuCounter] = bestPair;
		tabuCounter += 1;
		tabuCounter %= tabuSize;
		elementsOnTabu += 1;
		if(elementsOnTabu > tabuSize)
			elementsOnTabu = tabuSize;

		lastEnhance += 1;

		if(road_changed) {
			road = swap_2_opt(road, bestPair.first, bestPair.second);
			length = best_length;
			//printf("(%d)", lastEnhance);
			lastEnhance = 0;
			// If found best solution for now
			if(best_length < globalBestLen) {
				globalBestLen = best_length;
				globalBestRoad = road;
				printf("New local Minimum!!! : %ld\n", globalBestLen);
				lastEnhance = 0;
			}
		}

		// When long stagnation, perform kick
		kikCond = (lastEnhance >= enhancementLimit) ? true : false;
		if(kikCond) {
			//printf("Kik ");
			size_t shuffleRange = kickRange.second - kickRange.first;
			size_t shuffleSize = rng2() % shuffleRange + kickRange.first;
			size_t startPoint = rng2() % (n - kickRange.second - 1);
			road = doParametrizedKick(road, n, shuffleSize, startPoint);
			best_length = calculate_length(road, matrix, n);
			length = best_length;
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;
		//printf("%d..%d\n", stop1, kikCond);

	} while (stop1);

	return globalBestRoad;
}

std::pair<std::vector<std::pair<int, int>>, std::vector<std::vector<int>>> get_tabu_road_visual(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange) {

	std::vector<std::pair<int, int>> changeList;
	std::vector<std::vector<int>> startRoadChangeList;
	startRoadChangeList.emplace_back(road);
	size_t best_length = calculate_length(road, matrix, n);
	size_t length = best_length, computed_length;
	bool road_changed = true;

	size_t globalBestLen = best_length;
	std::vector<int> globalBestRoad = road;

	bool stop1 = true, kikCond = true;
	int lastEnhance = 0;

	std::pair<size_t, size_t> bestPair;
	std::pair<size_t, size_t> tabuList[tabuSize];
	for(int i = 0; i < tabuSize; i++)
		tabuList[i] = std::pair<size_t, size_t> {n + 1, n + 1};
	int tabuCounter = 0, elementsOnTabu = 0;

	auto start = std::chrono::high_resolution_clock::now();
	do {

		road_changed = false;
		for (size_t i = 0; i < n - 1; i++) {
			for (size_t j = i + 2; j < n - 1; j++) {
				//TODO: add aspiration!!!
				if(checkTabuList(tabuList, elementsOnTabu, i, j) == false) {
					//printf("%ld, %ld\n", i, j);
					computed_length = swap_length(length, road, matrix, i, j, n);
					if (computed_length < best_length) {
						//printf("b - %ld ", computed_length);
						bestPair = {i, j};
						best_length = computed_length;
						road_changed = true;
					}
				}
			}
		}

		tabuList[tabuCounter] = bestPair;
		tabuCounter += 1;
		tabuCounter %= tabuSize;
		elementsOnTabu += 1;
		if(elementsOnTabu > tabuSize)
			elementsOnTabu = tabuSize;

		lastEnhance += 1;

		if(road_changed) {
			road = swap_2_opt(road, bestPair.first, bestPair.second);
			length = best_length;
			changeList.emplace_back(bestPair.first, bestPair.second);
			//printf("(%d)", lastEnhance);
			lastEnhance = 0;
			// If found best solution for now
			if(best_length < globalBestLen) {
				globalBestLen = best_length;
				globalBestRoad = road;
				printf("New local Minimum!!! : %ld\n", globalBestLen);
				lastEnhance = 0;
			}
		}

		// When long stagnation, perform kick
		kikCond = (lastEnhance >= enhancementLimit) ? true : false;
		if(kikCond) {
			//printf("Kik ");
			size_t shuffleRange = kickRange.second - kickRange.first;
			size_t shuffleSize = rng2() % shuffleRange + kickRange.first;
			size_t startPoint = rng2() % (n - kickRange.second - 1);
			road = doParametrizedKick(road, n, shuffleSize, startPoint);
			best_length = calculate_length(road, matrix, n);
			length = best_length;
			startRoadChangeList.emplace_back(road);
			changeList.emplace_back(-1, -1);
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;
		//printf("%d..%d\n", stop1, kikCond);

	} while (stop1);

	return {changeList, startRoadChangeList};
}