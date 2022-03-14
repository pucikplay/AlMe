//
// Created by Franek on 12.03.2022.
//

#include "roadSolver.h"
#include <stdio.h>

std::vector<int> firstRoadMaker(std::vector<std::pair<double, double>> coords, int** matrix, int run) {

	size_t n = coords.size();

	std::vector<int> roadList;
	roadList.emplace_back(0);

	int currentPoint = 0;
	bool visited[n];

	for (size_t i = 0; i < n; i ++) {
		visited[i] = false;
	}

	visited[currentPoint] = true;

	if (run == 2) {

	} else {

		for(int i = 0; i < n; i++) {

			//printf("%d - %d\n", i, currentPoint);

			int nearestPoint = currentPoint;
			int smallestDist = __INT_MAX__;
			//printf("%d\n", smallestDist);

			for(int j = 0; j < n; j++) {
				int smaller = (currentPoint <= j) ? currentPoint : j;
				int bigger = (currentPoint > j) ? currentPoint : j;

				//if(i == 0) {
				//	printf("Now:\nj-%d\nvis-%d\nval-%d\n", j, visited[j], matrix[smaller][bigger]);
				//}
				if(j!= currentPoint && !visited[j] && matrix[smaller][bigger] != -1 && matrix[smaller][bigger] < smallestDist) {
					smallestDist = matrix[smaller][bigger];
					nearestPoint = j;
				}
				//if(i == 0) {
				//	printf("%d\n\n", smallestDist);
				//}
			}

			currentPoint = nearestPoint;
			roadList.emplace_back(currentPoint);
			visited[currentPoint] = true;
		}
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
	}

	for(int i = end; i <= roadList.size(); i++)
		newRoadList.emplace_back(roadList[i]);

	return newRoadList;
}