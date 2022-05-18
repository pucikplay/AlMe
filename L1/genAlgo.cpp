//
// Created by Franek on 13.05.2022.
//
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <random>

#include "genAlgo.h"
#include "roadSolver.h"
#include "k_random.h"

typedef std::vector<std::vector<int>> populationStruct;
typedef std::pair<std::vector<int>, std::vector<int>> vecPair;
std::mt19937 rngMut(time(nullptr));

vecPair orderBasedCross(std::vector<int> parent1, std::vector<int> parent2) {

	int crossSize = 5;

	// Generate random positions
	int numVals[parent1.size()];
	for(int i = 0; i < parent1.size(); i++)
		numVals[i] = i;

	std::random_shuffle(numVals, numVals + parent1.size());

	// Remember vals at given positions
	int vals1[crossSize], vals2[crossSize];
	for(int i = 0; i < crossSize; i++) {
		//printf("(%d)", numVals[i]);
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

vecPair ModifiedOrderCross(std::vector<int> parent1, std::vector<int> parent2) {

	int crossSize = 5;
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

populationStruct doCrossover(populationStruct parents, int size, int crossMode) {

	populationStruct children;
	vecPair childs;

	std::mt19937 rngC(time(nullptr));
	std::shuffle(std::begin(parents), std::end(parents), rngC);

	for(int i = 0; i < size; i += 2) {
		/*if(i == 0) {
			for(int j = 0; j < parents[0].size(); j++)
				printf("%d, ", parents[0][j]);
			printf("\n");
			for(int j = 0; j < parents[1].size(); j++)
				printf("%d, ", parents[1][j]);
			printf("\n\n");
		}*/

		if(crossMode == 0) childs = orderBasedCross(parents[i], parents[i + 1]);
		else if(crossMode == 1) childs = ModifiedOrderCross(parents[i], parents[i + 1]);
		/*if(i == 0) {
			for(int j = 0; j < childs.first.size(); j++)
				printf("%d, ", childs.first[j]);
			printf("\n");
			for(int j = 0; j < childs.second.size(); j++)
				printf("%d, ", childs.second[j]);
			printf("\n\n");
		}*/
		children.emplace_back(childs.first);
		children.emplace_back(childs.second);
	}

	if(parents.size() % 2 == 1)
		children.emplace_back(parents[parents.size()]);

	return children;
}

populationStruct doChildrenMutation(populationStruct children, double threshold, int size) {

	std::uniform_real_distribution<double> unif(0.0, 1.0);
	
	for(int i = 0; i < children.size(); i++) {
		if(unif(rngMut) < threshold) {
			int x1 = rngMut() % (size - 1);
			int x2 = rngMut() % (size - 1);
			int smaller = x1 < x2 ? x1 : x2;
			int bigger = x1 >= x2 ? x1 : x2;
			std::reverse(children[i].begin() + smaller + 1, children[i].begin() + bigger + 1);
			printf("(%d)", i);
		}
	}

	return children;
}

populationStruct doSelection(populationStruct parents, populationStruct children, int** matrix, size_t n) {

	std::vector<std::pair<size_t, std::vector<int>>> bigPop;
	std::pair<size_t, std::vector<int>> resultPair;
	populationStruct smallPop;
	size_t score;

	for(int i = 0; i < parents.size(); i++) {
		score = calculate_length(parents[i], matrix, n);
		resultPair = {score, parents[i]};
		bigPop.emplace_back(resultPair);
	}

	for(int i = 0; i < children.size(); i++) {
		score = calculate_length(children[i], matrix, n);
		resultPair = {score, children[i]};
		bigPop.emplace_back(resultPair);
	}

	sort(bigPop.begin(), bigPop.end());

	// Only best are going further
	for(int i = 0; i < parents.size(); i++)
		smallPop.emplace_back(bigPop[i].second);

	printf("Best: %ld; ", bigPop[0].first);

	return smallPop;
}

std::vector<int> geneticMain(size_t n, int** matrix, int populationSize, double mutationThreshold, int iterations, int crossMode) {

	std::vector<int> road = best_random_road(10, n, matrix);
	populationStruct population, children;

	for(int i = 0; i < populationSize * 2; i++)
		population.emplace_back(best_random_road(10, n, matrix));

	for(int i = 0; i < iterations; i++) {
		printf("\nIteration %d; ", i);
		population = doSelection(population, children, matrix, n);
		children = doCrossover(population, populationSize, crossMode);
		printf("Child Mutations: ");
		children = doChildrenMutation(children, mutationThreshold, n);
	}

	printf("\n Last Check; ");
	population = doSelection(population, children, matrix, n);

	//choosing best at the end
	size_t bestScore = SIZE_MAX, score;
	for(int i = 0; i < population.size(); i++) {
		score = calculate_length(population[i], matrix, n);
		if(score < bestScore) {
			bestScore = score;
			road = population[i];
		}
	}

	return road;
}
