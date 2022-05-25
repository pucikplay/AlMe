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

typedef std::vector<std::vector<int>> populationStruct;
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

	std::mt19937 rngC(time(nullptr));
	std::shuffle(std::begin(parents), std::end(parents), rngC);

	for(int i = 0; i < size; i += 2) {

		if(crossMode == 0) childs = orderBasedCross(parents[i], parents[i + 1], crossSize);
		else if(crossMode == 1) childs = modifiedOrderCross(parents[i], parents[i + 1], crossSize);
		else if(crossMode == 2) childs = partiallMappedyCross(parents[i], parents[i + 1], crossSize);
		children.emplace_back(childs.first);
		children.emplace_back(childs.second);
	}

	if(parents.size() % 2 == 1)
		children.emplace_back(parents[parents.size()]);

	return children;
}

populationStruct randomizedCrossover(populationStruct parents, int size, int crossMode, int crossSize, int crossCount) {

	populationStruct children;
	vecPair childs;

	int par1 = 0, par2 = 0;

	for(int i = 0; i < crossCount; i += 1) {

		do {
			par1 = rngMut() % size;
			par2 = rngMut() % size;
		} while(par1 == par2);

		if(crossMode == 0) childs = orderBasedCross(parents[par1], parents[par2], crossSize);
		else if(crossMode == 1) childs = modifiedOrderCross(parents[par1], parents[par2], crossSize);
		else if(crossMode == 2) childs = partiallMappedyCross(parents[par1], parents[par2], crossSize);
		children.emplace_back(childs.first);
		children.emplace_back(childs.second);
	}

	return children;
}

populationStruct doCrossover(populationStruct parents, int size, int crossMode, int crossSize, int crossType, int crossCount) {

	populationStruct children;

	if(crossType == 0) children = orderizedCrossover(parents, size, crossMode, crossSize);
	else if(crossType == 1) children = randomizedCrossover(parents, size, crossMode, crossSize, crossCount);

	return children;
}

populationStruct doChildrenMutation(populationStruct children, double threshold, int size, int mutMode, int intensification) {

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

				if(modeMut == 0) std::reverse(children[i].begin() + smaller + 1, children[i].begin() + bigger + 1);
				else if(modeMut == 1) {
					std::reverse(children[i].begin() + smaller, children[i].begin() + bigger + 1);
					std::reverse(children[i].begin() + smaller + 1, children[i].begin() + bigger + 1);
				}
				else if(modeMut == 2) {
					std::reverse(children[i].begin() + smaller, children[i].begin() + bigger + 1);
					std::reverse(children[i].begin() + smaller + 1, children[i].begin() + bigger);
				}
				if(printFlag) printf("%d", modeMut);
			}
			if(printFlag) printf(")");
		}
	}

	return children;
}

populationStruct bestSelection(populationStruct parents, populationStruct children, int populationSize, int** matrix, size_t n) {

	std::vector<std::pair<size_t, std::vector<int>>> bigPop;
	std::pair<size_t, std::vector<int>> resultPair;
	populationStruct smallPop;
	size_t score;

	// Adding parents
	for(int i = 0; i < parents.size(); i++) {
		score = calculate_length_asm(parents[i], matrix, n);
		resultPair = {score, parents[i]};
		bigPop.emplace_back(resultPair);
	}

	// Adding childrens
	for(int i = 0; i < children.size(); i++) {
		score = calculate_length_asm(children[i], matrix, n);
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

populationStruct semiRandomSelection(populationStruct parents, populationStruct children, int populationSize, int** matrix, size_t n) {

	std::vector<std::pair<size_t, std::vector<int>>> bigPop;
	std::pair<size_t, std::vector<int>> resultPair;
	populationStruct smallPop;
	size_t score;

	// Adding parents
	for(int i = 0; i < parents.size(); i++) {
		score = calculate_length_asm(parents[i], matrix, n);
		resultPair = {score, parents[i]};
		bigPop.emplace_back(resultPair);
	}

	// Adding childrens
	for(int i = 0; i < children.size(); i++) {
		score = calculate_length_asm(children[i], matrix, n);
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

	if(printFlag) printf("Best: %ld; ", calculate_length_asm(smallPop[0], matrix, n));

	return smallPop;
}

populationStruct doSelection(populationStruct parents, populationStruct children, int populationSize, int** matrix, size_t n, int selectionMode) {

	populationStruct smallPop;

	if(selectionMode == 0) smallPop = bestSelection(parents, children, populationSize, matrix, n);
	else if(selectionMode == 1) smallPop = semiRandomSelection(parents, children, populationSize, matrix, n);

	return smallPop;
}

std::vector<int> geneticMain(size_t n, int** matrix, int populationSize, double mutationThreshold, int mutMode, int muttionIntensification, int iterations, int crossMode, int crossSize, int crossType, int crossCount, int selectionMode) {

	std::vector<int> road = best_random_road(10, n, matrix);
	populationStruct population, children;

	for(int i = 0; i < populationSize * 2; i++)
		population.emplace_back(best_random_road(10, n, matrix));

	for(int i = 0; i < iterations; i++) {
		if(printFlag) printf("\nIteration %d; ", i);
		population = doSelection(population, children, populationSize, matrix, n, selectionMode);
		children = doCrossover(population, populationSize, crossMode, crossSize, crossType, crossCount);
		if(printFlag) printf("Child Mutations: ");
		children = doChildrenMutation(children, mutationThreshold, n, mutMode, muttionIntensification);
	}

	if(printFlag) printf("\n Last Check; ");
	population = doSelection(population, children, populationSize, matrix, n, selectionMode);

	//choosing best at the end
	size_t bestScore = SIZE_MAX, score;
	for(int i = 0; i < population.size(); i++) {
		score = calculate_length_asm(population[i], matrix, n);
		if(score < bestScore) {
			bestScore = score;
			road = population[i];
		}
	}

	return road;
}

std::pair<std::vector<int>, int> geneticMainTimed(size_t n, int** matrix, int populationSize, double mutationThreshold, int mutMode, int muttionIntensification, double time, int crossMode, int crossSize, int crossType, int crossCount, int selectionMode, int startMode) {

	clock_t start, end;
	double elapsed = 0.0;
	int iterationCounter = 0;
	populationStruct population, children;

	start = clock();
	// Only random
	if(startMode == 0)
		for(int i = 0; i < populationSize * 2; i++)
			population.emplace_back(best_random_road(10, n, matrix));
	// Only 'good' (Nearest Neighbor)
	else if(startMode == 1)
		for(int i = 0; i < populationSize * 2; i++)
			population.emplace_back(doNearestNeighbor(n, matrix, rngMut() % n));
	// Hybrid (With proportions 1:5)
	else if(startMode == 2)
		for(int i = 0; i < populationSize * 2; i++) {
			if(i % 6 == 0) population.emplace_back(doNearestNeighbor(n, matrix, rngMut() % n));
			else population.emplace_back(best_random_road(10, n, matrix));
		}

	std::vector<int> road = population[0];

	while(elapsed < time) {
		if(printFlag) printf("\nIteration %d; ", iterationCounter);
		population = doSelection(population, children, populationSize, matrix, n, selectionMode);
		children = doCrossover(population, populationSize, crossMode, crossSize, crossType, crossCount);
		if(printFlag) printf("Child Mutations: ");
		children = doChildrenMutation(children, mutationThreshold, n, mutMode, muttionIntensification);

		elapsed = double(clock() - start) / CLOCKS_PER_SEC;
		iterationCounter += 1;
	}

	if(printFlag) printf("\n Last Check; ");
	population = doSelection(population, children, populationSize, matrix, n, selectionMode);

	//choosing best at the end
	size_t bestScore = SIZE_MAX, score;
	for(int i = 0; i < population.size(); i++) {
		score = calculate_length_asm(population[i], matrix, n);
		if(score < bestScore) {
			bestScore = score;
			road = population[i];
		}
	}

	return {road, iterationCounter};
}

void geneticThread(populationStruct &population, size_t n, int** matrix, int populationSize, double mutationThreshold, int mutMode, int muttionIntensification, int iterations, int crossMode, int crossSize, int crossType, int crossCount, int selectionMode, int threadID) {
    populationStruct children;

    for(int i = 0; i < iterations; i++) {
        if(printFlag) printf("\nThread: %d, Iteration %d;", threadID, i);
        population = doSelection(population, children, populationSize, matrix, n, selectionMode);
        children = doCrossover(population, populationSize, crossMode, crossSize, crossType, crossCount);
        if(printFlag) printf("Child Mutations: ");
        children = doChildrenMutation(children, mutationThreshold, n, mutMode, muttionIntensification);
    }

    population = doSelection(population, children, populationSize, matrix, n, selectionMode);
}

std::vector<int> geneticIslands(size_t n, int** matrix, int populationSize, double mutationThreshold, int mutMode, int muttionIntensification, int wholeIterations, int crossMode, int crossSize, int crossType, int crossCount, int selectionMode, int islandsNumber, int swappingInterval, int swapSize) {
    //auto *islands = new populationStruct[islandsNumber];
    islandStruct islands(islandsNumber);
    populationStruct population;
    std::thread threads[islandsNumber];
    std::vector<int> road;
    size_t bestScore = SIZE_MAX, score;
    populationStruct swapPool;
    int idx;

    for (size_t i = 0; i < islandsNumber; i++) {
        for (size_t j = 0; j < populationSize * 2; j++) {
            islands[i].emplace_back(best_random_road(10, n, matrix));
        }
    }

    for (size_t i = 0; i < wholeIterations; i++) {
        for (size_t j = 0; j < islandsNumber; j++) {
            threads[j] = std::thread(geneticThread, std::ref(islands[j]), n, matrix, populationSize, mutationThreshold, mutMode, muttionIntensification, swappingInterval, crossMode, crossSize, crossType, crossCount, selectionMode, j);
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
            score = calculate_length_asm(islands[i][j], matrix, n);
            if(score < bestScore) {
                bestScore = score;
                road = islands[i][j];
            }
        }
        if(printFlag) printf("\n");
    }

    return road;
}
