//
// Created by Franek on 13.05.2022.
//
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <random>
#include <thread>

#include "genAlgo.h"
#include "roadSolver.h"
#include "k_random.h"

typedef std::vector<std::pair<std::vector<int>, int>> populationStruct;
typedef std::vector<populationStruct> islandStruct;
typedef std::pair<std::vector<int>, std::vector<int>> vecPair;
std::mt19937 rngMut(time(nullptr));

bool printFlag = false;

vecPair orderBasedCross(std::vector<int> parent1, std::vector<int> parent2, int crossSize) {

	// Generate random positions
	int numVals[parent1.size()];
	for(int i = 0; i < parent1.size(); i++)
		numVals[i] = i;

	std::random_shuffle(numVals, numVals + parent1.size());

	// Remember vals at given positions
	int vals1[crossSize], vals2[crossSize];
	for(int i = 0; i < crossSize; i++) {
		//if(printFlag) printf("(%d)", numVals[i]);
		vals1[i] = parent1[numVals[i]];
		vals2[i] = parent2[numVals[i]];
	}

	bool par1Flag, par2Flag;
	int val1Counter = 0, val2Counter = 0;

	// Crossing
	for(int i = 0; i < parent1.size(); i++) {

		par1Flag = false;
		par2Flag = false;
		for(int j = 0; j < crossSize; j++) {
			if(parent1[i] == vals2[j])
				par1Flag = true;
			if(parent2[i] == vals1[j])
				par2Flag = true;
		}
		if(par1Flag) {
			parent1[i] = vals2[val2Counter];
			val2Counter += 1;
		}
		if(par2Flag) {
			parent2[i] = vals1[val1Counter];
			val1Counter += 1;
		}
	}

	return {parent1, parent2};
}

vecPair modifiedOrderCross(std::vector<int> parent1, std::vector<int> parent2, int crossSize) {

	int divideStart = rngMut() % (parent1.size() - crossSize);

	// Remember vals at given positions
	int vals1[crossSize], vals2[crossSize];
	for(int i = 0; i < crossSize; i++) {
		vals1[i] = parent1[divideStart + i];
		vals2[i] = parent2[divideStart + i];
	}

	// Remembering places at sliced positions
	std::vector<int> child1, child2;
	int par1Val, par2Val;
	for(int i = 0; i < parent1.size(); i++) {

		par1Val = -1;
		par2Val = -1;

		for(int j = 0; j < crossSize; j++) {
			if(parent1[i] == vals2[j])
				par1Val = vals2[j];
			if(parent2[i] == vals1[j])
				par2Val = vals1[j];
		}
		child1.emplace_back(par1Val);
		child2.emplace_back(par2Val);

	}

	// Filling rest
	int par1Counter = 0, par2Counter = 0;

	for(int i = 0; i < parent1.size(); i++) {

		// Avoiding sliced part
		if(par1Counter == divideStart) par1Counter += crossSize;
		if(par2Counter == divideStart) par2Counter += crossSize;

		if(child1[i] == -1) {
			child1[i] = parent2[par2Counter];
			par2Counter += 1;
		}
		if(child2[i] == -1) {
			child2[i] = parent1[par1Counter];
			par1Counter += 1;
		}
	}

	return {child1, child2};
}

vecPair partiallMappedyCross(std::vector<int> parent1, std::vector<int> parent2, int crossSize) {

	int divideStart = rngMut() % (parent1.size() - crossSize);

	// Remember vals at given positions
	int vals1[crossSize], vals2[crossSize];
	for(int i = 0; i < crossSize; i++) {
		vals1[i] = parent1[divideStart + i];
		vals2[i] = parent2[divideStart + i];
	}

	// Remembering places at sliced positions
	std::vector<int> child1, child2;
	int par1Val, par2Val;
	for(int i = 0; i < parent1.size(); i++) {

		// 'intersecting' part
		if(divideStart <= i && i < divideStart + crossSize) {
			child1.emplace_back(parent2[i]);
			child2.emplace_back(parent1[i]);
			continue;
		}

		par1Val = parent1[i];
		par2Val = parent2[i];

		//child1 part
		bool isBad = true;
		while(isBad) {
			isBad = false;
			for(int j = 0; j < crossSize; j++) {
				if(par1Val == vals2[j]) {
					par1Val = vals1[j];
					isBad = true;
				}
			}
		}

		//child2 part
		isBad = true;
		while(isBad) {
			isBad = false;
			for(int j = 0; j < crossSize; j++) {
				if(par2Val == vals1[j]) {
					par2Val = vals2[j];
					isBad = true;
				}
			}
		}
		child1.emplace_back(par1Val);
		child2.emplace_back(par2Val);
	}

	return {child1, child2};
}

populationStruct orderizedCrossover(populationStruct parents, int size, int crossMode, int crossSize) {

	populationStruct children;
	vecPair childs;
	std::pair<std::vector<int>, int> child1, child2;

	std::mt19937 rngC(time(nullptr));
	std::shuffle(std::begin(parents), std::end(parents), rngC);

	for(int i = 0; i < size; i += 2) {

		if(crossMode == 0) childs = orderBasedCross(parents[i].first, parents[i + 1].first, crossSize);
		else if(crossMode == 1) childs = modifiedOrderCross(parents[i].first, parents[i + 1].first, crossSize);
		else if(crossMode == 2) childs = partiallMappedyCross(parents[i].first, parents[i + 1].first, crossSize);
		child1 = {childs.first, 0};
		child2 = {childs.second, 0};
		children.emplace_back(child1);
		children.emplace_back(child2);
	}

	if(parents.size() % 2 == 1)
		children.emplace_back(parents[parents.size()]);

	return children;
}

populationStruct randomizedCrossover(populationStruct parents, int size, int crossMode, int crossSize, int crossCount) {

	populationStruct children;
	vecPair childs;
	std::pair<std::vector<int>, int> child1, child2;

	int par1 = 0, par2 = 0;

	for(int i = 0; i < crossCount; i += 1) {

		do {
			par1 = rngMut() % size;
			par2 = rngMut() % size;
		} while(par1 == par2);

		if(crossMode == 0) childs = orderBasedCross(parents[par1].first, parents[par2].first, crossSize);
		else if(crossMode == 1) childs = modifiedOrderCross(parents[par1].first, parents[par2].first, crossSize);
		else if(crossMode == 2) childs = partiallMappedyCross(parents[par1].first, parents[par2].first, crossSize);
		child1 = {childs.first, 0};
		child2 = {childs.second, 0};
		children.emplace_back(child1);
		children.emplace_back(child2);
	}

	return children;
}

populationStruct doCrossover(populationStruct parents, int size, int crossMode, int crossSize, int crossType, int crossCount) {

	populationStruct children;

	if(crossType == 0) children = orderizedCrossover(parents, size, crossMode, crossSize);
	else if(crossType == 1) children = randomizedCrossover(parents, size, crossMode, crossSize, crossCount);
	else if(crossType == 2) children = randomizedCrossover(parents, size, crossMode, crossSize, size / 2);

	return children;
}

std::vector<int> localEnhanceMutation(std::vector<int> roadList, int** matrix, int start) {

	std::vector<int> newRoadList;
	int range = 5;
	int localPerm[range][range];
	if(start > roadList.size() - 5) start = roadList.size() - 5;

	// creating local mini-matrix
	for(int i = start; i < start + range; i++)
		for(int j = start; j < start + range; j++)
			localPerm[i - start][j - start] = matrix[roadList[i]][roadList[j]];

	// adding first part
	for(int i = 0; i <= start; i++)
		newRoadList.emplace_back(roadList[i]);

	int road1 = localPerm[0][1] + localPerm[1][2] + localPerm[2][3] + localPerm[3][4];
	int town1 = roadList[start + 1];
	int town2 = roadList[start + 2];
	int town3 = roadList[start + 3];
	int minDist = road1;

	int road2 = localPerm[0][1] + localPerm[1][3] + localPerm[3][2] + localPerm[2][4];
	if(road2 < minDist) {
		town1 = roadList[start + 1];
		town2 = roadList[start + 3];
		town3 = roadList[start + 2];
	}
	int road3 = localPerm[0][2] + localPerm[2][3] + localPerm[3][1] + localPerm[1][4];
	if(road3 < minDist) {
		town1 = roadList[start + 2];
		town2 = roadList[start + 3];
		town3 = roadList[start + 1];
	}
	int road4 = localPerm[0][2] + localPerm[2][1] + localPerm[1][3] + localPerm[3][4];
	if(road4 < minDist) {
		town1 = roadList[start + 2];
		town2 = roadList[start + 1];
		town3 = roadList[start + 3];
	}
	int road5 = localPerm[0][3] + localPerm[3][2] + localPerm[2][1] + localPerm[1][4];
	if(road5 < minDist) {
		town1 = roadList[start + 3];
		town2 = roadList[start + 2];
		town3 = roadList[start + 1];
	}
	int road6 = localPerm[0][3] + localPerm[3][1] + localPerm[1][2] + localPerm[2][4];
	if(road6 < minDist) {
		town1 = roadList[start + 3];
		town2 = roadList[start + 1];
		town3 = roadList[start + 2];
	}

	newRoadList.emplace_back(town1);
	newRoadList.emplace_back(town2);
	newRoadList.emplace_back(town3);

	for(int i = start + 4; i < roadList.size(); i++)
		newRoadList.emplace_back(roadList[i]);

	return newRoadList;
}

populationStruct doChildrenMutation(populationStruct children, int** matrix, double threshold, int size, int mutMode, int intensification, double enhanceChance) {

	std::uniform_real_distribution<double> unif(0.0, 1.0);
	
	for(int i = 0; i < children.size(); i++) {
		if(unif(rngMut) < threshold) {
			int mutationVal = 1 + rngMut() % intensification;
			if(printFlag) printf("(%d-%d-", i, mutationVal);

			for(int j = 0; j < mutationVal; j++) {

				int x1 = rngMut() % (size - 1);
				int x2 = rngMut() % (size - 1);
				int smaller = x1 < x2 ? x1 : x2;
				int bigger = x1 >= x2 ? x1 : x2;

				// 0 - Invert, 1 - Insert, 2 - Swap, 3 - Random
				int modeMut = mutMode;
				if(mutMode == 3) modeMut = rngMut() % 3;

				if(modeMut == 0) std::reverse(children[i].first.begin() + smaller + 1, children[i].first.begin() + bigger + 1);
				else if(modeMut == 1) {
					std::reverse(children[i].first.begin() + smaller, children[i].first.begin() + bigger + 1);
					std::reverse(children[i].first.begin() + smaller + 1, children[i].first.begin() + bigger + 1);
				}
				else if(modeMut == 2) {
					std::reverse(children[i].first.begin() + smaller, children[i].first.begin() + bigger + 1);
					std::reverse(children[i].first.begin() + smaller + 1, children[i].first.begin() + bigger);
				}

				if(unif(rngMut) < enhanceChance)
					for(int k = 0; k < children[i].first.size() - 5; k++)
						children[i].first = localEnhanceMutation(children[i].first, matrix, k);
				if(printFlag) printf("%d", modeMut);
			}
			if(printFlag) printf(")");
		}
	}

	return children;
}

populationStruct bestSelection(populationStruct parents, populationStruct children, int populationSize, int** matrix, size_t n, int ageMax) {

	std::vector<std::pair<size_t, std::pair<std::vector<int>, int>>> bigPop;
	std::pair<size_t, std::pair<std::vector<int>, int>> resultPair;
	populationStruct smallPop;
	size_t score;

	// Adding parents
	for(int i = 0; i < parents.size(); i++) {
		if(ageMax == -1 || parents[i].second < ageMax) {
			parents[i].second += 1;
			score = calculate_length_asm(parents[i].first, matrix, n);
			resultPair = {score, parents[i]};
			bigPop.emplace_back(resultPair);
		}
		else if(printFlag) printf("DIE OLD MAN!!!\n");
	}

	// Adding childrens
	for(int i = 0; i < children.size(); i++) {
		score = calculate_length_asm(children[i].first, matrix, n);
		resultPair = {score, children[i]};
		bigPop.emplace_back(resultPair);
	}

	// Sorting all
	sort(bigPop.begin(), bigPop.end());

	// Only best are going further
	for(int i = 0; i < populationSize; i++)
		smallPop.emplace_back(bigPop[i].second);

	if(printFlag) printf("Best: %ld; ", bigPop[0].first);

	return smallPop;
}

populationStruct semiRandomSelection(populationStruct parents, populationStruct children, int populationSize, int** matrix, size_t n, int ageMax) {

	std::vector<std::pair<size_t, std::pair<std::vector<int>, int>>> bigPop;
	std::pair<size_t, std::pair<std::vector<int>, int>> resultPair;
	populationStruct smallPop;
	size_t score;

	// Adding parents
	for(int i = 0; i < parents.size(); i++) {
		if(ageMax == -1 || parents[i].second < ageMax) {
			parents[i].second += 1;
			score = calculate_length_asm(parents[i].first, matrix, n);
			resultPair = {score, parents[i]};
			bigPop.emplace_back(resultPair);
		}
		else if(printFlag) printf("DIE OLD MAN!!!\n");
	}

	// Adding childrens
	for(int i = 0; i < children.size(); i++) {
		score = calculate_length_asm(children[i].first, matrix, n);
		resultPair = {score, children[i]};
		bigPop.emplace_back(resultPair);
	}

	// Sorting all
	sort(bigPop.begin(), bigPop.end());

	// Best always goes further
	smallPop.emplace_back(bigPop[0].second);

	// Giving reverse weights to square (^2) (worst = 1, best = many(1000))
	int summarizer = 0;
	int weightTable[bigPop.size() - 1];
	int popVal;
	for(int i = 1; i < bigPop.size(); i++) {
		popVal = (bigPop.size() - i) * (bigPop.size() - i);
		weightTable[i - 1] = popVal;
		summarizer += popVal;
	}

	// Taking random in each step
	int val, counter;
	for(int i = 1; i < populationSize; i++) {

		// Choosing Species
		val = rngMut() % summarizer;
		counter = -1;
		while(val >= 0) {
			counter += 1;
			val -= weightTable[counter];
		}

		// removing species from selection
		summarizer -= weightTable[counter];
		weightTable[counter] = 0;

		smallPop.emplace_back(bigPop[counter].second);
	}

	if(printFlag) printf("Best: %ld; ", calculate_length_asm(smallPop[0].first, matrix, n));

	return smallPop;
}

populationStruct doSelection(populationStruct parents, populationStruct children, int populationSize, int** matrix, size_t n, int selectionMode, int ageMax) {

	populationStruct smallPop;

	if(selectionMode == 0) smallPop = bestSelection(parents, children, populationSize, matrix, n, ageMax);
	else if(selectionMode == 1) smallPop = semiRandomSelection(parents, children, populationSize, matrix, n, ageMax);

	return smallPop;
}

std::vector<int> geneticMain(size_t n, int** matrix, int populationSize, double mutationThreshold, int mutMode, int muttionIntensification, double enhanceChance, int iterations, int crossMode, int crossSize, int crossType, int crossCount, int selectionMode, int ageMax) {

	std::vector<int> road = best_random_road(10, n, matrix);
	populationStruct population, children;

	for(int i = 0; i < populationSize * 2; i++) {
		std::pair<std::vector<int>, int> guy = {best_random_road(10, n, matrix), 0};
		population.emplace_back(guy);
	}

	for(int i = 0; i < iterations; i++) {
		if(printFlag) printf("\nIteration %d; ", i);
		population = doSelection(population, children, populationSize, matrix, n, selectionMode, ageMax);
		children = doCrossover(population, populationSize, crossMode, crossSize, crossType, crossCount);
		if(printFlag) printf("Child Mutations: ");
		children = doChildrenMutation(children, matrix, mutationThreshold, n, mutMode, muttionIntensification, enhanceChance);
	}

	if(printFlag) printf("\n Last Check; ");
	population = doSelection(population, children, populationSize, matrix, n, selectionMode, ageMax);

	//choosing best at the end
	size_t bestScore = SIZE_MAX, score;
	for(int i = 0; i < population.size(); i++) {
		score = calculate_length_asm(population[i].first, matrix, n);
		if(score < bestScore) {
			bestScore = score;
			road = population[i].first;
		}
	}

	return road;
}

std::pair<std::vector<int>, int> geneticMainTimed(size_t n, int** matrix, int populationSize, double mutationThreshold, int mutMode, int muttionIntensification, double enhanceChance, double time, int crossMode, int crossSize, int crossType, int crossCount, int selectionMode, int startMode, int ageMax) {

	clock_t start, end;
	double elapsed = 0.0;
	int iterationCounter = 0;
	populationStruct population, children;
	std::pair<std::vector<int>, int> guy;

	start = clock();
	// Only random
	if(startMode == 0)
		for(int i = 0; i < populationSize * 2; i++) {
			guy = {best_random_road(10, n, matrix), 0};
			population.emplace_back(guy);
		}
	// Only 'good' (Nearest Neighbor)
	else if(startMode == 1)
		for(int i = 0; i < populationSize * 2; i++) {
			guy = {doNearestNeighbor(n, matrix, rngMut() % n), 0};
			population.emplace_back(guy);
		}
	// Hybrid (With proportions 1:5)
	else if(startMode == 2)
		for(int i = 0; i < populationSize * 2; i++) {
			if(i % 5 == 0) guy = {doNearestNeighbor(n, matrix, rngMut() % n), 0};
			else guy = {best_random_road(10, n, matrix), 0};
			population.emplace_back(guy);
		}

	std::vector<int> road = population[0].first;

	while(elapsed < time) {
		if(printFlag) printf("\nIteration %d; ", iterationCounter);
		population = doSelection(population, children, populationSize, matrix, n, selectionMode, ageMax);
		children = doCrossover(population, populationSize, crossMode, crossSize, crossType, crossCount);
		if(printFlag) printf("Child Mutations: ");
		children = doChildrenMutation(children, matrix, mutationThreshold, n, mutMode, muttionIntensification, enhanceChance);

		elapsed = double(clock() - start) / CLOCKS_PER_SEC;
		iterationCounter += 1;
	}

	if(printFlag) printf("\n Last Check; ");
	population = doSelection(population, children, populationSize, matrix, n, selectionMode, ageMax);

	//choosing best at the end
	size_t bestScore = SIZE_MAX, score;
	for(int i = 0; i < population.size(); i++) {
		score = calculate_length_asm(population[i].first, matrix, n);
		if(score < bestScore) {
			bestScore = score;
			road = population[i].first;
		}
	}

	return {road, iterationCounter};
}

void geneticThread(populationStruct &population, size_t n, int** matrix, int populationSize, double mutationThreshold, int mutMode, int muttionIntensification, double enhanceChance, int iterations, int crossMode, int crossSize, int crossType, int crossCount, int selectionMode, int ageMax, int threadID) {
	populationStruct children;

	for(int i = 0; i < iterations; i++) {
		if(printFlag) printf("\nThread: %d, Iteration %d;", threadID, i);
		population = doSelection(population, children, populationSize, matrix, n, selectionMode, ageMax);
		children = doCrossover(population, populationSize, crossMode, crossSize, crossType, crossCount);
		if(printFlag) printf("Child Mutations: ");
		children = doChildrenMutation(children, matrix, mutationThreshold, n, mutMode, muttionIntensification, enhanceChance);
	}

	population = doSelection(population, children, populationSize, matrix, n, selectionMode, ageMax);
}

std::vector<int> geneticIslands(size_t n, int** matrix, int populationSize, double mutationThreshold, int mutMode, int muttionIntensification, double enhanceChance, int wholeIterations, int crossMode, int crossSize, int crossType, int crossCount, int selectionMode, int ageMax, int islandsNumber, int swappingInterval, int swapSize) {
	//auto *islands = new populationStruct[islandsNumber];
	islandStruct islands(islandsNumber);
	populationStruct population;
	std::thread threads[islandsNumber];
	std::vector<int> road;
	size_t bestScore = SIZE_MAX, score;
	populationStruct swapPool;

	std::pair<std::vector<int>, int> guy;

	int idx;

	for (size_t i = 0; i < islandsNumber; i++) {
		for (size_t j = 0; j < populationSize * 2; j++) {
			guy = {best_random_road(10, n, matrix), 0};
			islands[i].emplace_back(guy);
		}
	}

	for (size_t i = 0; i < wholeIterations; i++) {
		for (size_t j = 0; j < islandsNumber; j++) {
			threads[j] = std::thread(geneticThread, std::ref(islands[j]), n, matrix, populationSize, mutationThreshold, mutMode, muttionIntensification, enhanceChance, swappingInterval, crossMode, crossSize, crossType, crossCount, selectionMode, ageMax, j);
		}

		for (size_t j = 0; j < islandsNumber; j++) {
			threads[j].join();
		}

		for (size_t j = 0; j < islandsNumber; j++) {
			for (size_t k = 0; k < swapSize; k++) {
				idx = rngMut() % islands[j].size();
				swapPool.emplace_back(islands[j][idx]);
				islands[j].erase(islands[j].begin() + idx);
			}
		}

		std::shuffle(std::begin(swapPool), std::end(swapPool), rngMut);

		idx = 0;
		for (size_t j = 0; j < islandsNumber; j++) {
			for (size_t k = 0; k < swapSize; k++) {
				islands[j].emplace_back(swapPool[idx]);
				idx++;
			}
		}
		swapPool.clear();
	}

	for (size_t i = 0; i < islandsNumber; i++) {
		for (size_t j = 0; j < islands[i].size(); j++) {
			score = calculate_length_asm(islands[i][j].first, matrix, n);
			if(score < bestScore) {
				bestScore = score;
				road = islands[i][j].first;
			}
		}
		if(printFlag) printf("\n");
	}

	return road;
}
