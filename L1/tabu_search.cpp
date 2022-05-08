//
// Created by Franek on 13.04.2022.
//

#include <cstdint>
#include "tabu_search.h"

typedef std::mt19937 MyRNG;
MyRNG rng2(time(nullptr));
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
	int smaller, bigger;

	if(i < j) {
		smaller = (road[(i - 1 + n) % n] < road[i]) ? road[(i - 1 + n) % n] : road[i];
		bigger = (road[(i - 1 + n) % n] >= road[i]) ? road[(i - 1 + n) % n] : road[i];
	} else {
		smaller = (road[(i + 1) % n] < road[i]) ? road[(i + 1) % n] : road[i];
		bigger = (road[(i + 1) % n] >= road[i]) ? road[(i + 1) % n] : road[i];
	}
	newLength -= matrix[smaller][bigger];

	smaller = (road[(j - 1 + n) % n] < road[j % n]) ? road[(j - 1 + n) % n] : road[j % n];
	bigger = (road[(j - 1 + n) % n] >= road[j % n]) ? road[(j - 1 + n) % n] : road[j % n];
	newLength -= matrix[smaller][bigger];

	smaller = (road[j] < road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	bigger = (road[j] >= road[(j + 1) % n]) ? road[j] : road[(j + 1) % n];
	newLength -= matrix[smaller][bigger];

	if(i < j) {
		smaller = (road[(i - 1 + n) % n] < road[j]) ? road[(i - 1 + n) % n] : road[j];
		bigger = (road[(i - 1 + n) % n] >= road[j]) ? road[(i - 1 + n) % n] : road[j];
	} else {
		smaller = (road[(i + 1) % n] < road[j]) ? road[(i + 1) % n] : road[j];
		bigger = (road[(i + 1) % n] >= road[j]) ? road[(i + 1) % n] : road[j];
	}
	newLength += matrix[smaller][bigger];

	smaller = (road[i] < road[j]) ? road[i] : road[j];
	bigger = (road[i] >= road[j]) ? road[i] : road[j];
	newLength += matrix[smaller][bigger];

	smaller = (road[(j - 1 + n) % n] < road[(j + 1) % n]) ? road[(j - 1 + n) % n] : road[(j + 1) % n];
	bigger = (road[(j - 1 + n) % n] >= road[(j + 1) % n]) ? road[(j - 1 + n) % n] : road[(j + 1) % n];
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
		if(i < j) {
			std::reverse(road.begin() + i, road.begin() + j + 1);
			std::reverse(road.begin() + i + 1, road.begin() + j + 1);
		} else {
			std::reverse(road.begin() + j, road.begin() + i + 1);
			std::reverse(road.begin() + j, road.begin() + i);
		}
	} else if(mode == 2) {
		std::reverse(road.begin() + i, road.begin() + j + 1);
		std::reverse(road.begin() + i + 1, road.begin() + j);
	}
	return road;
}

std::vector<int> parametrizedKick(std::vector<int> road, size_t n, std::pair<size_t, size_t> kickRange) {

	std::vector<int> shuffledPart, result;

	size_t shuffleRange = kickRange.second - kickRange.first;
	size_t shuffleSize = rng2() % shuffleRange + kickRange.first;
	size_t startPoint = rng2() % (n - kickRange.second - 1);

	for(int i = startPoint; i < shuffleSize + startPoint; i++)
		shuffledPart.emplace_back(road[i]);
	
	std::shuffle(std::begin(shuffledPart), std::end(shuffledPart), rng2);

	for(int i = 0; i < startPoint; i++)
		result.emplace_back(road[i]);

	for(int i = 0; i < shuffleSize; i++)
		result.emplace_back(shuffledPart[i]);

	for(int i = shuffleSize + startPoint; i < n; i++)
		result.emplace_back(road[i]);

	return result;
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

bool checkTabuList(std::pair<size_t, size_t> *tabuList, size_t tabuSize, size_t i, size_t j) {
	//printf("%ld - %ld\n", i, j);
	for(int k = 0; k < tabuSize; k++)
		if(tabuList[k].first == i && tabuList[k].second == j) {
			//printf("Tabu! ");
			return true;
		}
	return false;
}

std::pair<size_t, std::pair<size_t, size_t>> checkNeighbourhood(std::vector<int> road, int** matrix, std::size_t n, size_t length, std::pair<size_t, size_t>* tabuList, int elementsOnTabu, int mode, size_t globalBestLen) {

	std::pair<size_t, size_t> bestPair;
	size_t bestLen = -1, computedLen;

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
				}
			} else if (computedLen < globalBestLen) {
				//printf("Aspiration!!! : (%ld)\n", computedLen);
				bestPair = {i, j};
				bestLen = computedLen;
			}

			//When in insert mode, then check second possibility
			if(mode == 9) {
				computedLen = insertLength(length, road, matrix, j, i, n);
				if(checkTabuList(tabuList, elementsOnTabu, j, i) == false) {
					if (computedLen < bestLen || bestLen == -1) {
						//printf("b - %ld ", computedLen);
						bestPair = {j, i};
						bestLen = computedLen;
					}
				} else if (computedLen < globalBestLen) {
					//printf("Aspiration!!! : (%ld)\n", computedLen);
					bestPair = {j, i};
					bestLen = computedLen;
				}
			}
		}
	}

	return {bestLen, bestPair};
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

std::vector<int> get_tabu_road(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode) {

	size_t length = calculate_length(road, matrix, n);

	size_t globalBestLen = length;
	std::vector<int> globalBestRoad = road;
	int globalBestTabuCounter = 0;

	bool stop1 = true;
	bool firstRun = true;
	bool devHelpFlag = false;
	bool devHelpKikFlag = false;
	int lastEnhance = 0, rseed = 7;

	std::pair<size_t, size_t> bestPair;
	std::pair<size_t, size_t>* tabuList = new std::pair<size_t, size_t>[tabuSize];
	std::pair<size_t, size_t>* globBestTabuList = new std::pair<size_t, size_t>[tabuSize];
	for(int i = 0; i < tabuSize; i++)
		tabuList[i] = std::pair<size_t, size_t> {n + 1, n + 1};
	int tabuCounter = 0, elementsOnTabu = 0;

	std::vector<structLTM> returnList;
	int counterLTM = 0;

	auto start = std::chrono::high_resolution_clock::now();
	do {

		//{bestLen, bestPair}
		std::pair<size_t, std::pair<size_t, size_t>> res = checkNeighbourhood(road, matrix, n, length, tabuList, elementsOnTabu, mode, globalBestLen);
		bestPair = res.second;

		if(mode == 1) tabuList[tabuCounter] = {bestPair. second, bestPair.first};
		else tabuList[tabuCounter] = bestPair;
		tabuCounter += 1;
		tabuCounter %= tabuSize;
		elementsOnTabu += 1;
		if(elementsOnTabu > tabuSize)
			elementsOnTabu = tabuSize;

		lastEnhance += 1;

		//Step doing and interpretation
		road = doStep(road, bestPair.first, bestPair.second, mode);
		length = res.first;

		// If found best solution remember it
		if(length < globalBestLen) {
			globalBestLen = length;
			globalBestRoad = road;
			globalBestTabuCounter = tabuCounter;
			if(firstRun)
				for(int i = 0; i < tabuSize; i++)
					globBestTabuList[i] = tabuList[i];

			if (devHelpFlag) printf("New Minimum!!! : %ld\n", globalBestLen);
			if(!firstRun) {
				if (devHelpFlag) printf("Adding to LTM (After %d cycles)\n", lastEnhance);
				//Adding to Long-term
				structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {tabuList, tabuCounter}};
				returnList = addThingToLTM(returnList, newThingOnLTM);
				counterLTM = returnList.size();
			}
			lastEnhance = 0;
		}

		// When long stagnation, go back to place from LTM
		if(lastEnhance >= enhancementLimit) {
			//First best local minimum (only happens once in the beginning)
			if(firstRun) {
				if (devHelpFlag) printf("Adding First Local Min to LTM\n");
				structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {globBestTabuList, globalBestTabuCounter}};
				returnList.emplace_back(newThingOnLTM);
				firstRun = false;
			}
			//If there is no nodes in LTM left, perform kik
			else if(counterLTM == 0) {
				if (devHelpKikFlag) printf(" (Kik) ");
				road = parametrizedKick(road, n, kickRange);
				length = calculate_length(road, matrix, n);
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
				length = calculate_length(road, matrix, n);
				if (devHelpFlag) printf("Go back to %ld\n", lastThingOnLTM.first.second);
			}
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;

	} while (stop1);

	return globalBestRoad;
}

std::vector<int> deterministicTabu(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, int mode, int kikMode, int kikSize) {

	size_t length = calculate_length(road, matrix, n);

	size_t globalBestLen = length;
	std::vector<int> globalBestRoad = road;
	int globalBestTabuCounter = 0;

	bool stop1 = true;
	bool firstRun = true;
	bool devHelpFlag = false;
	bool devHelpKikFlag = false;
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

		//{bestLen, bestPair}
		std::pair<size_t, std::pair<size_t, size_t>> res = checkNeighbourhood(road, matrix, n, length, tabuList, elementsOnTabu, mode, globalBestLen);
		bestPair = res.second;

		if(mode == 1) tabuList[tabuCounter] = {bestPair. second, bestPair.first};
		else tabuList[tabuCounter] = bestPair;
		tabuCounter += 1;
		tabuCounter %= tabuSize;
		elementsOnTabu += 1;
		if(elementsOnTabu > tabuSize)
			elementsOnTabu = tabuSize;

		lastEnhance += 1;

		//Step doing and interpretation
		road = doStep(road, bestPair.first, bestPair.second, mode);
		length = res.first;

		// If found best solution remember it
		if(length < globalBestLen) {
			globalBestLen = length;
			globalBestRoad = road;
			globalBestTabuCounter = tabuCounter;
			if(firstRun)
				for(int i = 0; i < tabuSize; i++)
					globBestTabuList[i] = tabuList[i];

			if (devHelpFlag) printf("New Minimum!!! : %ld\n", globalBestLen);
			if(!firstRun) {
				if (devHelpFlag) printf("Adding to LTM (After %d cycles)\n", lastEnhance);
				//Adding to Long-term
				structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {tabuList, tabuCounter}};
				returnList = addThingToLTM(returnList, newThingOnLTM);
				counterLTM = returnList.size();
			}
			lastEnhance = 0;
		}

		// When long stagnation, go back to place from LTM
		if(lastEnhance >= enhancementLimit) {
			//First best local minimum (only happens once in the beginning)
			if(firstRun) {
				if (devHelpFlag) printf("Adding First Local Min to LTM\n");
				structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {globBestTabuList, globalBestTabuCounter}};
				returnList.emplace_back(newThingOnLTM);
				firstRun = false;
			}
			//If there is no nodes in LTM left, perform kik
			else if(counterLTM == 0) {
				if (devHelpKikFlag) printf(" (Kik) ");
				road = deterministicKik(road, rseed, kikMode, kikSize);
				rseed += 13;

				length = calculate_length(road, matrix, n);
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
				length = calculate_length(road, matrix, n);
				if (devHelpFlag) printf("Go back to %ld\n", lastThingOnLTM.first.second);
			}
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;

	} while (stop1);

	return globalBestRoad;
}

std::pair<std::vector<int>, int> deterministicTabuWithKikCount(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, int mode, int kikMode, int kikSize) {

	size_t length = calculate_length(road, matrix, n);

	size_t globalBestLen = length;
	std::vector<int> globalBestRoad = road;
	int globalBestTabuCounter = 0;

	bool stop1 = true;
	bool firstRun = true;
	bool devHelpFlag = false;
	bool devHelpKikFlag = false;
	int lastEnhance = 0, rseed = 7;

	std::pair<size_t, size_t> bestPair;
	std::pair<size_t, size_t>* tabuList = new std::pair<size_t, size_t>[tabuSize];
	std::pair<size_t, size_t>* globBestTabuList = new std::pair<size_t, size_t>[tabuSize];
	for(int i = 0; i < tabuSize; i++)
		tabuList[i] = std::pair<size_t, size_t> {n + 1, n + 1};
	int tabuCounter = 0, elementsOnTabu = 0;

	std::vector<structLTM> returnList;
	int counterLTM = 0;
	int kikCount = 0;
	//int mode = smode;

	auto start = std::chrono::high_resolution_clock::now();
	do {
		//mode += 1;
		//mode %= 3;

		//{bestLen, bestPair}
		std::pair<size_t, std::pair<size_t, size_t>> res = checkNeighbourhood(road, matrix, n, length, tabuList, elementsOnTabu, mode, globalBestLen);
		bestPair = res.second;

		if(mode == 1) tabuList[tabuCounter] = {bestPair. second, bestPair.first};
		else tabuList[tabuCounter] = bestPair;
		tabuCounter += 1;
		tabuCounter %= tabuSize;
		elementsOnTabu += 1;
		if(elementsOnTabu > tabuSize)
			elementsOnTabu = tabuSize;

		lastEnhance += 1;

		//Step doing and interpretation
		road = doStep(road, bestPair.first, bestPair.second, mode);
		length = res.first;

		// If found best solution remember it
		if(length < globalBestLen) {
			globalBestLen = length;
			globalBestRoad = road;
			globalBestTabuCounter = tabuCounter;
			if(firstRun)
				for(int i = 0; i < tabuSize; i++)
					globBestTabuList[i] = tabuList[i];

			if (devHelpFlag) printf("New Minimum!!! : %ld\n", globalBestLen);
			if(!firstRun) {
				if (devHelpFlag) printf("Adding to LTM (After %d cycles)\n", lastEnhance);
				//Adding to Long-term
				structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {tabuList, tabuCounter}};
				returnList = addThingToLTM(returnList, newThingOnLTM);
				counterLTM = returnList.size();
			}
			lastEnhance = 0;
		}

		// When long stagnation, go back to place from LTM
		if(lastEnhance >= enhancementLimit) {
			//First best local minimum (only happens once in the beginning)
			if(firstRun) {
				if (devHelpFlag) printf("Adding First Local Min to LTM\n");
				structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {globBestTabuList, globalBestTabuCounter}};
				returnList.emplace_back(newThingOnLTM);
				firstRun = false;
			}
			//If there is no nodes in LTM left, perform kik
			else if(counterLTM == 0) {
				if (devHelpKikFlag) printf(" (Kik) ");
				road = deterministicKik(road, rseed, kikMode, kikSize);
				rseed += 13;

				length = calculate_length(road, matrix, n);
				lastEnhance = 0;
				kikCount += 1;
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
				length = calculate_length(road, matrix, n);
				if (devHelpFlag) printf("Go back to %ld\n", lastThingOnLTM.first.second);
			}
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;

	} while (stop1);

	return {globalBestRoad, kikCount};
}


std::pair<std::vector<std::pair<int, int>>, std::vector<std::vector<int>>> get_tabu_road_visual(std::vector<int> road, int** matrix, std::size_t n, int tabuSize, double time, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode) {

	std::vector<std::pair<int, int>> changeList;
	std::vector<std::vector<int>> startRoadChangeList;
	startRoadChangeList.emplace_back(road);

	size_t length = calculate_length(road, matrix, n);

	size_t globalBestLen = length;
	std::vector<int> globalBestRoad = road;
	int globalBestTabuCounter = 0;

	bool stop1 = true;
	bool firstRun = true;
	bool devHelpFlag = true;
	bool devHelpKikFlag = false;
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

		//{bestLen, bestPair}
		std::pair<size_t, std::pair<size_t, size_t>> res = checkNeighbourhood(road, matrix, n, length, tabuList, elementsOnTabu, mode, globalBestLen);
		bestPair = res.second;

		if(mode == 1) tabuList[tabuCounter] = {bestPair. second, bestPair.first};
		else tabuList[tabuCounter] = bestPair;
		tabuCounter += 1;
		tabuCounter %= tabuSize;
		elementsOnTabu += 1;
		if(elementsOnTabu > tabuSize)
			elementsOnTabu = tabuSize;

		lastEnhance += 1;

		//Step doing and interpretation
		road = doStep(road, bestPair.first, bestPair.second, mode);
		length = res.first;
		changeList.emplace_back(bestPair.first, bestPair.second);

		// If found best solution remember it
		if(length < globalBestLen) {
			globalBestLen = length;
			globalBestRoad = road;
			globalBestTabuCounter = tabuCounter;
			if(firstRun)
				for(int i = 0; i < tabuSize; i++)
					globBestTabuList[i] = tabuList[i];

			if (devHelpFlag) printf("New Minimum!!! : %ld\n", globalBestLen);
			if(!firstRun) {
				if (devHelpFlag) printf("Adding to LTM (After %d cycles)\n", lastEnhance);
				//Adding to Long-term
				structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {tabuList, tabuCounter}};
				returnList = addThingToLTM(returnList, newThingOnLTM);
				counterLTM = returnList.size();
			}
			lastEnhance = 0;
		}

		// When long stagnation, go back to place from LTM
		if(lastEnhance >= enhancementLimit) {
			//First best local minimum (only happens once in the beginning)
			if(firstRun) {
				if (devHelpFlag) printf("Adding First Local Min to LTM\n");
				structLTM newThingOnLTM = {{globalBestRoad, globalBestLen}, {globBestTabuList, globalBestTabuCounter}};
				returnList.emplace_back(newThingOnLTM);
				firstRun = false;
			}
			//If there is no nodes in LTM left, perform kik
			else if(counterLTM == 0) {
				if (devHelpKikFlag) printf(" (Kik) ");
				road = parametrizedKick(road, n, kickRange);
				length = calculate_length(road, matrix, n);
				lastEnhance = 0;
				startRoadChangeList.emplace_back(road);
				changeList.emplace_back(-1, -1);
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
				length = calculate_length(road, matrix, n);
				if (devHelpFlag) printf("Go back to %ld\n", lastThingOnLTM.first.second);
				startRoadChangeList.emplace_back(road);
				changeList.emplace_back(-1, -1);
			}
		}

		stop1 = ((std::chrono::high_resolution_clock::now() - start).count() < time) ? true : false;

	} while (stop1);

	return {changeList, startRoadChangeList};
}