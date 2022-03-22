#include <iostream>
#include "parser.h"
#include "roadSolver.h"
#include "visualizer.h"
#include "random_generator.h"
#include "k_random.h"
#include "2_opt.h"
#include <time.h>

int main(int argc, char *argv[]) {

	std::cout << "Hello, Meta!" << std::endl;

	int** matrix;
	std::vector<std::pair<double, double>> coords;

	//Control flags
	bool euclideanFlag = true;
	bool randomFlag = false;
	bool drawFlag = false;
	int mode = 2; //0 - k-Random, 1 - NearestNeighbor, 2 - 2-Opt

	//Euclidean
	if(euclideanFlag) {
		//Generated
		if(randomFlag) {
			size_t n = 100;
			coords = gen_coords(n);
		} else {
			//coords = parse_coords("Data/Euc2D/a280.tsp");
			coords = parse_coords("Data/Euc2D/st70.tsp");
			//coords = parse_coords("Data/Euc2D/pr1002.tsp");
			//coords = parse_coords("Data/Euc2D/pr2392.tsp");
			matrix = coords_to_matrix(coords);
		}
	} else { //Matrix
		std::pair<int**, int> result;

		//Full Matrix
		//result = parse_matrix("Data/FullMatrix/swiss42.tsp");
		//result = parse_matrix("Data/FullMatrix/bays29.tsp");

		//Upper Row
		//result = parse_matrix("Data/UpperRow/brg180.tsp");
		result = parse_matrix("Data/UpperRow/brazil58.tsp");
		//result = parse_matrix("Data/UpperRow/bayg29.tsp");

		matrix = result.first;
		int n = result.second;
		coords = genCords(n);
	}
	if(drawFlag) {
		if(mode == 0)
			drawKRandom(coords, matrix, 100000); //Still poor
		else if(mode == 1)
			drawNearestNeigh(coords, matrix);
		else if(mode == 2)
			draw2Opt(coords, matrix);
	} else {
		if(mode == 0) {
			
			printf("Not now...\n");

		} else if(mode == 1) {
			
			printf("Not now...\n");

		} else if(mode == 2) {

			std::cout << "Generating first Road..." << std::endl;
			clock_t start = clock();
			//std::vector<int> roadList = best_random_road(10000, coords.size(), matrix);
			//std::vector<int> roadList = doNearestNeighbor(coords.size(), matrix, 0);
			//std::vector<int> roadList = bestStartingNeighbor(coords.size(), matrix);
			//std::vector<int> roadList = bestFullBranchingNeighbor(coords.size(), matrix); // Nie puszczać, gdy często są takie same!!!
			std::vector<int> roadList = bestBranchingNeighbor(coords.size(), matrix, 5); // 0 = zwykły nearest neighbor
			std::cout << "Generated!!! - Moving to 2-Opt making" << std::endl;

			clock_t middle = clock();
			std::vector<int> road = get_2_opt_road(roadList, matrix, roadList.size());
			std::cout << "2-Opt made!!!" << std::endl;
			
			clock_t almost = clock();
			size_t score = calculate_length(road, matrix, road.size());
			
			clock_t end = clock();

			double elapsed = double(end - start) / CLOCKS_PER_SEC;
			double elapsed1 = double(middle - start) / CLOCKS_PER_SEC;
			double elapsed2 = double(almost - middle) / CLOCKS_PER_SEC;
			double elapsed3 = double(end - almost) / CLOCKS_PER_SEC;

			printf("Final Road Cost: %ld\n", score);
			printf("Time Overall:			%f seconds.\n", elapsed);
			printf("Time For Generation of Start:	%f seconds.\n", elapsed1);
			printf("Time For 2-Opt Used:		%f seconds.\n", elapsed2);
			printf("Time For length calculation:	%f seconds.\n", elapsed3);
		}
	}
	//Coord Check
	/*for (size_t i = 0; i < n; i++) {
		printf("%ld %d %d\n", i, (int)coords[i].first, (int)coords[i].second);
	}*/

	//Matrix Check
	/*int counter = 0;
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < n; j++) {
			counter += 1;
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("%d\n", counter);*/

	printf("Bye!\n");

	return 0;
}
