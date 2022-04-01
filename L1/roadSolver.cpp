//
// Created by Franek on 12.03.2022.
//
#include <iostream>
#include <utility>
#include "roadSolver.h"
#include "k_random.h"
#include <stdio.h>

// Full, only for good data
std::vector<int> bestFullBranchingNeighbor(size_t n, int** matrix) {

	std::vector<int> roadList;
	std::vector<int> bestRoadList;
	size_t bestLength = SIZE_MAX;
	size_t length = 0;

	for (size_t i = 0; i < n; i++) {
		roadList = startFullBranchingNeighbor(n, matrix, i);
		length = calculate_length(roadList, matrix, n);

		if (length < bestLength) {
			bestLength = length;
			bestRoadList = roadList;
		}
	}

	return bestRoadList;
}

std::vector<int> startFullBranchingNeighbor(size_t n, int** matrix, int k) {

	std::vector<int> available;
	for(int i = 0; i < n; i++)
		if(i != k)
			available.emplace_back(i);

	std::vector<int> roadList = getFullNearestBranch(n, matrix, available, k);
	return roadList;
}

std::vector<int> getFullNearestBranch(size_t n, int** matrix, std::vector<int> available, int k) {

	std::vector<int> nextMoveList;
	std::vector<int> equalNearestList;
	size_t smallestDist = SIZE_MAX;

	for(int i = 0; i < available.size(); i++) {

		// Will be used later
		if(available[i] != k)
			nextMoveList.emplace_back(available[i]);

		int smaller = (k <= available[i]) ? k : available[i];
		int bigger = (k > available[i]) ? k : available[i];

		if(matrix[smaller][bigger] < smallestDist) {

			equalNearestList.clear();
			equalNearestList.emplace_back(available[i]);
			smallestDist = matrix[smaller][bigger];

		} else if(matrix[smaller][bigger] == smallestDist)
			equalNearestList.emplace_back(available[i]);
	}

	std::vector<int> tmpRoadList;
	std::vector<int> bestRoadList;
	int bestChoose = k;

	if(available.size() != 0) {

		size_t smallestScore = SIZE_MAX, tmpScore = 0;

		for(int i = 0; i < equalNearestList.size(); i++) {
			tmpRoadList = getFullNearestBranch(n, matrix, nextMoveList, equalNearestList[i]);
			tmpScore = calculate_length(tmpRoadList, matrix, tmpRoadList.size());
			if(tmpScore <= smallestScore) {
				smallestScore = tmpScore;
				bestChoose = equalNearestList[i];
				bestRoadList = tmpRoadList;
			}
		}
	}

	bestRoadList.emplace(bestRoadList.begin(), k);
	return bestRoadList;
}

// Faster
std::vector<int> bestBranchingNeighbor(size_t n, int** matrix, int maxDepth) {

	std::vector<int> roadList;
	std::vector<int> bestRoadList;
	size_t bestLength = SIZE_MAX;
	size_t length = 0;

	for (size_t i = 0; i < n; i++) {
		//std::cout << " " << i << ", " << std::endl;
		roadList = doBranchingNeighbor(n, matrix, i, maxDepth);
		//std::cout << " " << i << ", ";
		length = calculate_length(roadList, matrix, n);
		//std::cout << " " << i << ", ";

		if (length < bestLength) {
			bestLength = length;
			bestRoadList = roadList;
		}
	}

	return bestRoadList;
}

size_t doBranching(std::vector<int> roadList, size_t n, int point, int** matrix, int depthLeft) {
	
	if(depthLeft == 0 || roadList.size() >= n - 1)
		return 0;

	std::vector<int> newRoadList;
	bool visited[n];

	for (size_t i = 0; i < n; i ++)
		visited[i] = false;

	for(size_t i = 0; i < roadList.size(); i++) {
		visited[roadList[i]] = true;
		newRoadList.emplace_back(roadList[i]);
	}
	newRoadList.emplace_back(point);

	//Proper Branch
	std::vector<std::pair<int, size_t>> equalNearestList;
	size_t smallestDist = SIZE_MAX;

	for(int j = 0; j < n; j++) {

		int smaller = (point <= j) ? point : j;
		int bigger = (point > j) ? point : j;

		if(j!= point && !visited[j] && matrix[smaller][bigger] != -1) {
			if(matrix[smaller][bigger] < smallestDist) {

				smallestDist = matrix[smaller][bigger];
				equalNearestList.clear();
				equalNearestList.emplace_back(j, smallestDist);

			} else if(matrix[smaller][bigger] == smallestDist) {
				equalNearestList.emplace_back(j, smallestDist);
			}
		}
	}

	size_t score = 0;

	if(equalNearestList.size() > 1) {
			
		int bestEqualPoint = -1;
		size_t bestEqualDist = SIZE_MAX;

		for(int j = 0; j < equalNearestList.size(); j++) {
			size_t tmpDist = doBranching(newRoadList, n, equalNearestList[j].first, matrix, depthLeft - 1);
			if(tmpDist <= bestEqualDist) {
				bestEqualPoint = j;
				bestEqualDist = tmpDist;
			}
		}

		score = equalNearestList[bestEqualPoint].second + bestEqualDist;

	} else {
		size_t deepenDist = doBranching(newRoadList, n, equalNearestList[0].first, matrix, depthLeft - 1);
		score = equalNearestList[0].second + deepenDist;
	}

	return score;
}

std::vector<int> doBranchingNeighbor(size_t n, int** matrix, int k, int maxDepth) {

	std::vector<int> roadList;
	roadList.emplace_back(k);

	int currentPoint = k;
	bool visited[n];

	for (size_t i = 0; i < n; i ++)
		visited[i] = false;

	visited[currentPoint] = true;

	for(int i = 1; i < n; i++) {

		std::vector<int> equalNearestList;
		size_t smallestDist = SIZE_MAX;

		for(int j = 0; j < n; j++) {

			int smaller = (currentPoint <= j) ? currentPoint : j;
			int bigger = (currentPoint > j) ? currentPoint : j;

			if(j!= currentPoint && !visited[j] && matrix[smaller][bigger] != -1) {
				if(matrix[smaller][bigger] < smallestDist) {

					equalNearestList.clear();
					equalNearestList.emplace_back(j);
					smallestDist = matrix[smaller][bigger];

				} else if(matrix[smaller][bigger] == smallestDist) {
					equalNearestList.emplace_back(j);
				}
			}
		}

		if(equalNearestList.size() > 1) {
			
			int bestEqualPoint = -1;
			size_t bestEqualDist = SIZE_MAX;

			for(int j = 0; j < equalNearestList.size(); j++) {
				size_t tmpDist = doBranching(roadList, n, equalNearestList[j], matrix, maxDepth);
				if(tmpDist < bestEqualDist) {
					bestEqualPoint = equalNearestList[j];
					bestEqualDist = tmpDist;
				}
			}
			currentPoint = bestEqualPoint;

		} else
			currentPoint = equalNearestList[0];

		roadList.emplace_back(currentPoint);
		visited[currentPoint] = true;
	}

	return roadList;
}

void doBranchingThread(std::vector<int> roadList, size_t n, int point, int** matrix, int depthLeft, size_t *tmpDepths, int thread_no) {
    size_t tmpDepth = doBranching(std::move(roadList), n, point, matrix, depthLeft);
    tmpDepths[thread_no] = tmpDepth;
}

std::vector<int> doBranchingNeighborParallel(size_t n, int** matrix, int k, int maxDepth, int max_threads) {

    std::vector<int> roadList;
    roadList.emplace_back(k);

    int currentPoint = k;
    bool visited[n];

    for (size_t i = 0; i < n; i ++)
        visited[i] = false;

    visited[currentPoint] = true;

    for(int i = 1; i < n; i++) {

        std::vector<int> equalNearestList;
        size_t smallestDist = SIZE_MAX;

        for(int j = 0; j < n; j++) {

            int smaller = (currentPoint <= j) ? currentPoint : j;
            int bigger = (currentPoint > j) ? currentPoint : j;

            if(j!= currentPoint && !visited[j] && matrix[smaller][bigger] != -1) {
                if(matrix[smaller][bigger] < smallestDist) {

                    equalNearestList.clear();
                    equalNearestList.emplace_back(j);
                    smallestDist = matrix[smaller][bigger];

                } else if(matrix[smaller][bigger] == smallestDist) {
                    equalNearestList.emplace_back(j);
                }
            }
        }

        if(equalNearestList.size() > 1) {

            std::thread th[equalNearestList.size()];
            auto *tmpDists = new size_t[equalNearestList.size()];
            int bestEqualPoint = -1;
            size_t bestEqualDist = SIZE_MAX;

            for(int j = 0; j < equalNearestList.size(); j++) {
                th[j] = std::thread(doBranchingThread, roadList, n, equalNearestList[j], matrix, maxDepth, tmpDists, j);
            }
            for(int j = 0; j < equalNearestList.size(); j++) {
                th[j].join();
            }
            for(int j = 0; j < equalNearestList.size(); j++) {
                if(tmpDists[j] < bestEqualDist) {
                    bestEqualPoint = equalNearestList[j];
                    bestEqualDist = tmpDists[j];
                }
            }
            currentPoint = bestEqualPoint;

        } else
            currentPoint = equalNearestList[0];

        roadList.emplace_back(currentPoint);
        visited[currentPoint] = true;
    }

    return roadList;
}

// Simple branchless
std::vector<int> bestStartingNeighbor(size_t n, int** matrix) {

	std::vector<int> roadList;
	std::vector<int> bestRoadList;
	size_t bestLength = SIZE_MAX;
	size_t length = 0;

	for (size_t i = 0; i < n; i++) {
		roadList = doNearestNeighbor(n, matrix, i);
		length = calculate_length(roadList, matrix, n);

		if (length < bestLength) {
			bestLength = length;
			bestRoadList = roadList;
		}
	}

	return bestRoadList;
}

std::vector<int> doNearestNeighbor(size_t n, int** matrix, int k) {

	std::vector<int> roadList;
	roadList.emplace_back(k);

	int currentPoint = k;
	bool visited[n];

	for (size_t i = 0; i < n; i ++)
		visited[i] = false;

	visited[currentPoint] = true;

	for(int i = 1; i < n; i++) {

		int nearestPoint = currentPoint;
		int smallestDist = __INT_MAX__;

		for(int j = 0; j < n; j++) {
			int smaller = (currentPoint <= j) ? currentPoint : j;
			int bigger = (currentPoint > j) ? currentPoint : j;

			if(j!= currentPoint && !visited[j] && matrix[smaller][bigger] != -1 && matrix[smaller][bigger] < smallestDist) {
				smallestDist = matrix[smaller][bigger];
				nearestPoint = j;
			}
		}

		currentPoint = nearestPoint;
		roadList.emplace_back(currentPoint);
		visited[currentPoint] = true;
	}

	return roadList;
}

std::vector<int> localEnhancer(std::vector<int> roadList, int** matrix, int start, int end) {

	std::vector<int> newRoadList;
	int range = end - start + 1;
	int localPerm[range][range];

	for(int i = 0; i <= start; i++)
		newRoadList.emplace_back(roadList[i]);

	if(range == 5) {
		for(int i = start; i < start + range; i++)
			for(int j = i + 1; j < start + range; j++) {
				int partA = matrix[roadList[i]][roadList[j]];
				int partB = matrix[roadList[j]][roadList[i]];
				int biggerVal = (partA >= partB) ? partA : partB;
				localPerm[i - start][j - start] = biggerVal;
				localPerm[j - start][i - start] = biggerVal;
			}

		// Ugly, but works
		int road1 = localPerm[0][1] + localPerm[1][2] + localPerm[2][3] + localPerm[3][4];
		int town1 = roadList[start + 1], town2 = roadList[start + 2], town3 = roadList[start + 3], minDist = road1;

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
	} else {
		for(int i = start + 1; i < end; i++)
			newRoadList.emplace_back(roadList[i]);
	}

	for(int i = end; i < roadList.size(); i++)
		newRoadList.emplace_back(roadList[i]);

	return newRoadList;
}