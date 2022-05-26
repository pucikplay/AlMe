#include <iostream>
#include <string.h>
#include <string>
#include "parser.h"
#include "roadSolver.h"
#include "visualizer.h"
#include "random_generator.h"
#include "k_random.h"
#include "2_opt.h"
#include "3_opt.h"
#include "tabu_search.h"
#include "tabu_tester.h"
#include "tester.h"
#include "genAlgo.h"
#include "genAlgoTester.h"
#include <time.h>

int main(int argc, char *argv[]) {

	std::cout << "Hello, Meta!" << std::endl;

	int** matrix;
	std::vector<std::pair<double, double>> coords;

	//Control flags
	bool euclideanFlag = false;
	bool randomFlag = false;
	bool drawFlag = false;
	int mode = 4; //0 - k-Random, 1 - NearestNeighbor, 2 - 2-Opt, 3 - 3-Opt, 4 - tests, 5 - tabu, 6 - genetic

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
		//result = parse_matrix("Data/UpperRow/brazil58.tsp");
		result = parse_matrix("Data/UpperRow/bayg29.tsp");
		//result = parse_matrix("Data/Asimetric/ft70.atsp");

		matrix = result.first;
		int n = result.second;
		coords = genCords(n);
	}
	if(drawFlag) {
		if(mode == 0) drawKRandom(coords, matrix, 100000); //Still poor
		else if(mode == 1) drawNearestNeigh(coords, matrix);
		else if(mode == 2) draw2Opt(coords, matrix);
		else if(mode == 3) draw3Opt(coords, matrix);
		else if(mode == 5) {
//			size_t tabuSize, double time, size_t enhancedLimit, std::pair<size_t, size_t> kickRange
			int tabuSize = 7;
			size_t enhancedLimit = 15;
			double time = 32.0 * 1000000000;
			std::pair<size_t, size_t> kickRange = {8, 32};
			int neiMode = 0;

			drawTabu(coords, matrix, tabuSize, time, enhancedLimit, kickRange, neiMode);
		}
	} else {
		if(mode == 0) {
			
			printf("Not now...\n");

		} else if(mode == 1) {
			
			printf("Not now...\n");

		} else if(mode == 2 || mode == 3) {

			for(int i = 0; i <= 4; i++) {
				printf("\nBranching depth = %d\n\n", i);

				std::cout << "Generating first Road..." << std::endl;
				clock_t start = clock();
				//std::vector<int> roadList = best_random_road(10000, coords.size(), matrix);
				//std::vector<int> roadList = doNearestNeighbor(coords.size(), matrix, 0);
				//std::vector<int> roadList = bestStartingNeighbor(coords.size(), matrix);
				//std::vector<int> roadList = bestFullBranchingNeighbor(coords.size(), matrix); // Nie puszczać, gdy często są takie same!!!
				std::vector<int> roadList = bestBranchingNeighbor(coords.size(), matrix, i); // 0 = zwykły nearest neighbor
				std::cout << "Generated!!! - Moving to " << mode << "-Opt making" << std::endl;

				clock_t middle = clock();
				std::vector<int> road;
				if(mode == 2)
					road = get_2_opt_road(roadList, matrix, roadList.size());
				else if(mode == 3)
					road = get_3_opt_road(roadList, matrix, roadList.size());
				std::cout << mode << "-Opt made!!!" << std::endl;
				
				clock_t almost = clock();
				size_t score = calculate_length(road, matrix, road.size());
				size_t scoreBefore = calculate_length(roadList, matrix, road.size());
				
				clock_t end = clock();

				double elapsed = double(end - start) / CLOCKS_PER_SEC;
				double elapsed1 = double(middle - start) / CLOCKS_PER_SEC;
				double elapsed2 = double(almost - middle) / CLOCKS_PER_SEC;
				double elapsed3 = double(end - almost) / CLOCKS_PER_SEC;

				printf("Start Road Cost: %ld\n", scoreBefore);
				printf("Final Road Cost: %ld\n", score);
				printf("Time Overall:			%f seconds.\n", elapsed);
				printf("Time For Generation of Start:	%f seconds.\n", elapsed1);
				printf("Time For %d-Opt Used:		%f seconds.\n", mode, elapsed2);
				printf("Time For length calculation:	%f seconds.\n", elapsed3);

				// Setting 2 mins per solution as max
				if(elapsed > 120)
					break;
			}
		} else if (mode == 4) {
			//for(int i = 1; i < argc; i++) {
			//	std::string fileName = argv[i];	
			//	test_0_K_Rand(fileName);
			//}
			//test_0_0_TSPLIB("Tests/test00.txt");
			//test_0_1_TSPLIB("Tests/test01.txt");
			//test_0_2_TSPLIB("Tests/test02.txt");
			//test_1_2_TSPLIB("Tests/test12.txt");
			//test_2_2_TSPLIB("Tests/test22.txt");
			//test_Neigh_TSPLIB("Tests/testNeigh.txt");

			//parameter_tuner("Tests/TabuTests/parameterTuner.txt");

			/*for(int i = 1; i < argc; i++) {
				printf("%s\n", argv[i]);
				std::string fileName = argv[i];
				//mass_test_0_1_TSPLIB(fileName);
				//mass_test_0_2_TSPLIB(fileName);
				//mass_test_1_2_TSPLIB(fileName);
				//mass_test_02_12_TSPLIB(fileName);
				//mass_test_2_3_TSPLIB(fileName);
				//mass_test_Neigh_TSPLIB(fileName);
				mass_test_breachDepth_TSPLIB(fileName);
			}*/

			//geneticModeTestSimetric(argv[1]);
			geneticModeTestAsimetric(argv[1]);
		} else if (mode == 5) {

			for(int i = 0; i <= 2; i++) {

				if(i == 0) printf("Invert:\n");
				else if(i == 1) printf("\nInsert:\n");
				else if(i == 2) printf("\nSwap:\n");

				std::vector<int> road;
				int tabuSize = 7;
				size_t enhancementLimit = 15;
				double time = 4.0 * 1000000000;
				int mode = i; // 0 - invert, 1 - insert, 2 - swap
				int kikMode = i; // 0 - invert, 1 - insert, 2 - swap
				int kikSize = 7; // elements to 'shuffle' count
				std::pair<size_t, size_t> kickRange = {8, 32}; // range for random kick

				clock_t start = clock();
				std::vector<int> roadList = bestStartingNeighbor(coords.size(), matrix);
				//std::vector<int> roadList = best_random_road(10000, coords.size(), matrix);
				//std::vector<int> roadList = doNearestNeighbor(coords.size(), matrix, 0);
				clock_t middle = clock();
				
				//road = get_tabu_road(roadList, matrix, roadList.size(), tabuSize, time, enhancementLimit, kickRange, mode);
				road = deterministicTabu(roadList, matrix, roadList.size(), tabuSize, time, enhancementLimit, mode, kikMode, kikSize);
				clock_t almost = clock();
				size_t score = calculate_length(road, matrix, road.size());
				size_t scoreBefore = calculate_length(roadList, matrix, road.size());
				clock_t end = clock();

				double elapsed = double(end - start) / CLOCKS_PER_SEC;
				double elapsed1 = double(middle - start) / CLOCKS_PER_SEC;
				double elapsed2 = double(almost - middle) / CLOCKS_PER_SEC;
				double elapsed3 = double(end - almost) / CLOCKS_PER_SEC;

				printf("\nStart Road Cost: %ld\n", scoreBefore);
				printf("Final Road Cost: %ld\n", score);
				printf("Time Overall:			%f seconds.\n", elapsed);
				printf("Time For Generation of Start:	%f seconds.\n", elapsed1);
				printf("Time For Tabu Used:		%f seconds.\n", elapsed2);
				printf("Time For length calculation:	%f seconds.\n", elapsed3);

			}

		} else if(mode == 6) {
			//std::vector<int> road = geneticMain(coords.size(), matrix, 20, 0.05, 3, 5, 1000, 2, 7, 1, 21, 1);
			std::pair<std::vector<int>, int> result = geneticMainTimed(coords.size(), matrix, 20, 0.05, 3, 5, -1.0, 10.0, 2, 7, 1, 21, 1, 2);
			std::vector<int> road = result.first;
            //std::vector<int> road = geneticIslands(coords.size(), matrix, 20, 0.05, 3, 4, 10, 0, 5, 6, 10, 5);
			size_t score = calculate_length(road, matrix, road.size());

			printf("Best: %ld, Iterations: %d\n", score, result.second);

			//printf("Best: %ld\n", score);
		}
	}
	printf("Bye!\n");

	return 0;
}
