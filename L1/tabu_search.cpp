//
// Created by Franek on 13.04.2022.
//

#include <cstdint>
#include "tabu_search.h"

typedef std::mt19937 MyRNG;
MyRNG rng2(time(nullptr));
typedef std::pair<std::pair<bool, size_t>, std::pair<size_t, size_t>> neiResult;
// {roadVector, score}, {tabuList, tabuCounter}
typedef std::pair<std::pair<std::vector<int>, size_t>, std::pair<std::pair<size_t, size_t>*, int>> structLTM;

size_t invertLength(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t n) {

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

size_t insertLength(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t n) {

	size_t newLength = length;

	int smaller = (road[(i - 1 + n) % n] < road[i]) ? road[(i - 1 + n) % n] : road[i];
	int bigger = (road[(i - 1 + n) % n] >= road[i]) ? road[(i - 1 + n) % n] : road[i];
	newLength -= matrix[smaller][bigger];

	smaller = (road[j - 1] < road[(j) % n]) ? road[j - 1] : road[(j) % n];
	bigger = (road[j - 1] >= road[(j) % n]) ? road[j - 1] : road[(j) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[j] < road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	bigger = (road[j] >= road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[(i - 1 + n) % n] < road[j]) ? road[(i - 1 + n) % n] : road[j];
	bigger = (road[(i - 1 + n) % n] >= road[j]) ? road[(i - 1 + n) % n] : road[j];
	newLength += matrix[smaller][bigger];

	smaller = (road[i] < road[j]) ? road[i] : road[j];
	bigger = (road[i] >= road[j]) ? road[i] : road[j];
	newLength += matrix[smaller][bigger];

	smaller = (road[j - 1] < road[(j + 1) % n]) ? road[j - 1] : road[(j + 1) % n];
	bigger = (road[j - 1] >= road[(j + 1) % n]) ? road[j - 1] : road[(j + 1) % n];
	newLength += matrix[smaller][bigger];

	return newLength;
}

size_t swapLength(size_t length, const std::vector<int>& road, int** matrix, size_t i, size_t j, size_t n) {

	size_t newLength = length;

	if(i + 1 == j) {
		newLength = invertLength(length, road, matrix, (i - 1 + n) % n, j, n);
		return newLength;
	}

	int smaller = (road[(i - 1 + n) % n] < road[i]) ? road[(i - 1 + n) % n] : road[i];
	int bigger = (road[(i - 1 + n) % n] >= road[i]) ? road[(i - 1 + n) % n] : road[i];
	newLength -= matrix[smaller][bigger];

	smaller = (road[i] < road[(i + 1) % n]) ? road[i] : road[(i + 1) % n];
	bigger = (road[i] >= road[(i + 1) % n]) ? road[i] : road[(i + 1) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[j - 1] < road[(j) % n]) ? road[j - 1] : road[(j) % n];
	bigger = (road[j - 1] >= road[(j) % n]) ? road[j - 1] : road[(j) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[j] < road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	bigger = (road[j] >= road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[(i - 1 + n) % n] < road[j]) ? road[(i - 1 + n) % n] : road[j];
	bigger = (road[(i - 1 + n) % n] >= road[j]) ? road[(i - 1 + n) % n] : road[j];
	newLength += matrix[smaller][bigger];

	smaller = (road[(i + 1) % n] < road[j]) ? road[(i + 1) % n] : road[j];
	bigger = (road[(i + 1) % n] >= road[j]) ? road[(i + 1) % n] : road[j];
	newLength += matrix[smaller][bigger];

	smaller = (road[j - 1] < road[i]) ? road[j - 1] : road[i];
	bigger = (road[j - 1] >= road[i]) ? road[j - 1] : road[i];
	newLength += matrix[smaller][bigger];

	smaller = (road[i] < road[(j + 1) % n]) ? road[i] : road[(j + 1) % n];
	bigger = (road[i] >= road[(j + 1) % n]) ? road[i] : road[(j + 1) % n];
	newLength += matrix[smaller][bigger];

	return newLength;
}

std::vector<int> doStep(std::vector<int> road, size_t i, size_t j, int mode) {

	if(mode == 0) {
		std::reverse(road.begin() + i + 1, road.begin() + j + 1);
	} else if(mode == 1) {
		std::reverse(road.begin() + i, road.begin() + j + 1);
		std::reverse(road.begin() + i + 1, road.begin() + j + 1);
	} else if(mode == 2) {
		std::reverse(road.begin() + i, road.begin() + j + 1);
		std::reverse(road.begin() + i + 1, road.begin() + j);
	}
	return road;
}

std::vector<int> parametrizedKick(std::vector<int> road, size_t n, size_t shuffleSize, size_t startingPoint) {

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

//Last place = best score
std::vector<structLTM> addThingToLTM(std::vector<structLTM> returnList, structLTM newThing) {
	int n = returnList.size();
	std::vector<structLTM> result;
	int counter = 0;

	while(counter < n && returnList[counter].first.second > newThing.first.second) {
		result.emplace_back(returnList[counter]);
		counter += 1;
	}
	result.emplace_back(newThing);
	while(counter < n) {
		result.emplace_back(returnList[counter]);
		counter += 1;
	}
	return result;
}

//Removes last element from vector
std::vector<structLTM> cutLast(std::vector<structLTM> returnList) {
	int n = returnList.size();
	std::vector<structLTM> result;
	int counter = 0;

	while(counter < n - 1) {
		result.emplace_back(returnList[counter]);
		counter += 1;
	}
	return result;
}

//based on 2_opt -> propably to delete/change
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
			}
		}

		// When long stagnation, perform kick
		kikCond = (lastEnhance >= enhancementLimit) ? true : false;
		if(kikCond) {
			//printf("Kik ");
			size_t shuffleRange = kickRange.second - kickRange.first;
			size_t shuffleSize = rng2() % shuffleRange + kickRange.first;
			size_t startPoint = rng2() % (n - kickRange.second - 1);
			road = parametrizedKick(road, n, shuffleSize, startPoint);
			best_length = calculate_length(road, matrix, n);
			length = best_length;
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;
		//printf("%d..%d\n", stop1, kikCond);

	} while (stop1);

	return globalBestRoad;
}

std::vector<int> deterministicKik(std::vector<int> road, int rseed, int mode, int size) {
	std::vector<int> result = road;

	int x1 = road.size() / 2 + 1;
	int x2 = road.size() - 1;

	std::vector<std::pair<int, int>> kikVec;
	for(int i = 0; i < size; i++) {

		x1 += (int)(sqrt(road.size()) * log2(road.size()));
		x1 %= (road.size() * road.size());
		x2 += (int)(sqrt(road.size()) * log2(road.size()));
		x2 %= (road.size() * road.size());

		int shuffle1 = (x1 * rseed) % road.size();
		int shuffle2 = (x2 * rseed) % road.size();

		if(shuffle1 < shuffle2) result = doStep(result, shuffle1, shuffle2, mode);
		else if(shuffle1 > shuffle2) result = doStep(result, shuffle2, shuffle1, mode);
	}

	return result;
}

neiResult checkNeighbourhood(std::vector<int> road, int** matrix, std::size_t n, size_t length, std::pair<size_t, size_t>* tabuList, int elementsOnTabu, int mode, size_t globalBestLen) {

	std::pair<size_t, size_t> bestPair;
	size_t bestLen = -1, computedLen;

	bool road_changed = false;
	for (size_t i = 0; i < n - 1; i++) {
		for (size_t j = i + 1; j < n - 1; j++) {
			//printf("%ld, %ld\n", i, j);

			if(mode == 0) computedLen = invertLength(length, road, matrix, i, j, n);
			else if(mode == 1) computedLen = insertLength(length, road, matrix, i, j, n);
			else if(mode == 2) computedLen = swapLength(length, road, matrix, i, j, n);

			if(checkTabuList(tabuList, elementsOnTabu, i, j) == false) {
				if (computedLen < bestLen || bestLen == -1) {
					//printf("b - %ld ", computedLen);
					bestPair = {i, j};
					bestLen = computedLen;
					road_changed = true;
				}
			} else if (computedLen < globalBestLen) {
				//printf("Aspiration!!! : (%ld)\n", computedLen);
				bestPair = {i, j};
				bestLen = computedLen;
				road_changed = true;
			}
		}
	}

	return {{road_changed, bestLen}, bestPair};
}

std::vector<int> deterministicTabu(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, int mode, int kikMode, int kikSize) {

	size_t best_length = calculate_length(road, matrix, n);
	size_t length = best_length, computed_length;
	bool road_changed = true;

	size_t globalBestLen = best_length;
	std::vector<int> globalBestRoad = road;
	int globalBestTabuCounter = 0;

	bool stop1 = true;
	bool firstRun = true;
	bool devHelpFlag = true;
	int lastEnhance = 0, rseed = 7;

	std::pair<size_t, size_t> bestPair;
	std::pair<size_t, size_t>* tabuList = new std::pair<size_t, size_t>[tabuSize];
	std::pair<size_t, size_t>* globBestTabuList = new std::pair<size_t, size_t>[tabuSize];
	for(int i = 0; i < tabuSize; i++)
		tabuList[i] = std::pair<size_t, size_t> {n + 1, n + 1};
	int tabuCounter = 0, elementsOnTabu = 0;

	std::vector<structLTM> returnList;
	int counterLTM = 0;
	//int mode = smode;

	auto start = std::chrono::high_resolution_clock::now();
	do {
		//mode += 1;
		//mode %= 3;

		neiResult res = checkNeighbourhood(road, matrix, n, length, tabuList, elementsOnTabu, mode, globalBestLen);
		road_changed = res.first.first;
		bestPair = res.second;

		tabuList[tabuCounter] = bestPair;
		tabuCounter += 1;
		tabuCounter %= tabuSize;
		elementsOnTabu += 1;
		if(elementsOnTabu > tabuSize)
			elementsOnTabu = tabuSize;

		lastEnhance += 1;

		if(road_changed) {
			road = doStep(road, bestPair.first, bestPair.second, mode);
			best_length = res.first.second;
			length = best_length;
			//printf("(%d)", lastEnhance);
			//printf("Change: %ld\n", length);
			// If found best solution for now
			if(best_length < globalBestLen) {
				globalBestLen = best_length;
				globalBestRoad = road;
				globalBestTabuCounter = tabuCounter;
				if(firstRun)
					for(int i = 0; i < tabuSize; i++)
						globBestTabuList[i] = tabuList[i];

				if (devHelpFlag) printf("New Minimum!!! : %ld\n", globalBestLen);
				if(lastEnhance > 1 && !firstRun) {
					if (devHelpFlag) printf("Adding to LTM (After %d cycles)\n", lastEnhance);
					//Adding to Long-term
					structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {tabuList, tabuCounter}};
					returnList = addThingToLTM(returnList, newThingOnLTM);
					counterLTM = returnList.size();
				}
				lastEnhance = 0;
			}
		}

		// When long stagnation, go back to place from LTM
		if(lastEnhance >= enhancementLimit) {
			//First best local minimum
			if(firstRun) {
				if (devHelpFlag) printf("Adding First Local Min to LTM\n");
				structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {globBestTabuList, globalBestTabuCounter}};
				returnList.emplace_back(newThingOnLTM);
				firstRun = false;
			}
			//If there is no nodes in LTM left, perform kik
			else if(counterLTM == 0) {
				road = deterministicKik(road, rseed, kikMode, kikSize);
				rseed += 13;

				best_length = calculate_length(road, matrix, n);
				length = best_length;
				lastEnhance = 0;
			}
			//Go one place back on LTM list
			else {
				structLTM lastThingOnLTM;
				counterLTM -= 1;
				lastThingOnLTM = returnList[counterLTM];
				returnList = cutLast(returnList);
				for(int i = 0; i < tabuSize; i++)
					tabuList[i] = lastThingOnLTM.second.first[i];
				road = lastThingOnLTM.first.first;
				if (devHelpFlag) printf("Go back to %ld\n", lastThingOnLTM.first.second);
			}
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;

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
			}
		}

		// When long stagnation, perform kick
		kikCond = (lastEnhance >= enhancementLimit) ? true : false;
		if(kikCond) {
			printf(" (Kik) ");
			size_t shuffleRange = kickRange.second - kickRange.first;
			size_t shuffleSize = rng2() % shuffleRange + kickRange.first;
			size_t startPoint = rng2() % (n - kickRange.second - 1);
			road = parametrizedKick(road, n, shuffleSize, startPoint);
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