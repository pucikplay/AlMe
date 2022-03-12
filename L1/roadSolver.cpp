//
// Created by Franek on 12.03.2022.
//

#include "roadSolver.h"
#include <stdio.h>

std::vector<std::pair<std::pair<double, double>, std::pair<double, double>>> firstRoadMaker(std::vector<std::pair<double, double>> coords, int** matrix, int run) {
	std::vector<std::pair<std::pair<double, double>, std::pair<double, double>>> roadList;

	size_t n = coords.size();

	int currentPoint = 0;
	bool visited[n];

	for (size_t i = 0; i < n; i ++) {
		visited[i] = false;
	}

	visited[currentPoint] = true;

	if (run == 2) {

	} else {

		for(int i = 0; i < n; i++) {

			printf("%d - %d\n", i, currentPoint);

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

			roadList.emplace_back(coords[currentPoint], coords[nearestPoint]);
			currentPoint = nearestPoint;
			visited[currentPoint] = true;
		}
		roadList.emplace_back(coords[currentPoint], coords[0]);
	}

	return roadList;
}