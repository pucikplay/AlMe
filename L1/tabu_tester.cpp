//
// Created by Franek on 05.05.2022.
//

#include "tabu_tester.h"
#include <string.h>
#include <string>
#include "tabu_multithreaded_wrapper.h"

#define test_no 3

//Comparison Part
void tabu_neiComp_T(const std::string& file) {

	int** matrix;
	std::vector<std::pair<double, double>> coords;

	coords = parse_coords("Data/Euc2D/st70.tsp");
	//coords = parse_coords("Data/Euc2D/a280.tsp");
	//coords = parse_coords("Data/Euc2D/pr1002.tsp");

	matrix = coords_to_matrix(coords);

	std::ofstream File(file);

	for(int i = 0; i <= 2; i++) {

		/*if(i == 0) printf("Invert:\n");
		else if(i == 1) printf("\nInsert:\n");
		else if(i == 2) printf("\nSwap:\n");*/

		std::vector<int> road;
		int deterMinistic = 0;
		int tabuSize = 7;
		size_t enhancementLimit = 15;
		int timeInt = 4;
		double time = (double)timeInt * 1000000000;
		int mode = i; // 0 - invert, 1 - insert, 2 - swap
		int kikMode = i; // 0 - invert, 1 - insert, 2 - swap
		int kikSize = 7; // elements to 'shuffle' count
		std::pair<size_t, size_t> kickRange = {8, 32}; // range for random kick

		clock_t start = clock();
		std::vector<int> roadList = bestStartingNeighbor(coords.size(), matrix);
		//std::vector<int> roadList = best_random_road(10000, coords.size(), matrix);
		//std::vector<int> roadList = doNearestNeighbor(coords.size(), matrix, 0);
		clock_t middle = clock();
		
		if (deterMinistic == 0) road = get_tabu_road(roadList, matrix, roadList.size(), tabuSize, time, enhancementLimit, kickRange, mode);
		if (deterMinistic == 1) road = deterministicTabu(roadList, matrix, roadList.size(), tabuSize, time, enhancementLimit, mode, kikMode, kikSize);

		clock_t almost = clock();
		size_t score = calculate_length(road, matrix, road.size());
		size_t scoreBefore = calculate_length(roadList, matrix, road.size());
		clock_t end = clock();

		double elapsed = double(end - start) / CLOCKS_PER_SEC;
		double elapsed1 = double(middle - start) / CLOCKS_PER_SEC;
		double elapsed2 = double(almost - middle) / CLOCKS_PER_SEC;
		double elapsed3 = double(end - almost) / CLOCKS_PER_SEC;

		/*printf("\nStart Road Cost: %ld\n", scoreBefore);
		printf("Final Road Cost: %ld\n", score);
		printf("Time Overall:			%f seconds.\n", elapsed);
		printf("Time For Generation of Start:	%f seconds.\n", elapsed1);
		printf("Time For Tabu Used:		%f seconds.\n", elapsed2);
		printf("Time For length calculation:	%f seconds.\n", elapsed3);*/

		File << coords.size() << ";" << mode << ";" << kikMode << ";" << kikSize << ";";
		File << tabuSize << ";" << enhancementLimit << ";" << timeInt << ";" << deterMinistic << ";";
	}
}

void test_variants_random(const std::string& file, int init_rode, int tabuSize, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode)
{
    std::vector<int> road;
    std::vector<int> roadList;
    std::ofstream File(file);
    int** matrix;
    std::vector<std::pair<double, double>> coords;
    size_t n;
    double time;

    File << time << ";" << init_rode << ";" << enhancementLimit << ";" << \
    kickRange.first << ";" << kickRange.second << ";" << mode << "\n";

    for (size_t i = 0; i < 3; i++) {

        if (i == 0) {
            coords = parse_coords("../Data/Euc2D/st70.tsp");
            n = 70;
            time = double(n * n * 2000);
        }
        if (i == 1) {
            coords = parse_coords("../Data/Euc2D/a280.tsp");
            n = 280;
            time = double(n * n * 2000);
        }
        if (i == 2) {
            coords = parse_coords("../Data/Euc2D/pr1002.tsp");
            n = 1002;
            time = double(n * n * 2000);
        }

        matrix = coords_to_matrix(coords);

        if (init_rode == 0) roadList = bestStartingNeighbor(n, matrix);
        if (init_rode == 1) roadList = best_random_road(10000, n, matrix);
        if (init_rode == 2) roadList = doNearestNeighbor(n, matrix, 0);

        road = get_tabu_road(roadList, matrix, n, tabuSize, time, enhancementLimit, kickRange, mode);

        size_t score = calculate_length(road, matrix, n);

        File << n << ";" << time << ";" << score << "\n";
    }
}

void test_variants_deterministic(const std::string& file, int init_rode, int tabuSize, size_t enhancementLimit, int mode, int kikMode, int kikSize)
{
    std::vector<int> road;
    std::vector<int> roadList;
    std::ofstream File(file);
    int** matrix;
    std::vector<std::pair<double, double>> coords;
    size_t n;
    double time;

    File << init_rode << ";" << enhancementLimit << ";" << \
    mode << ";" << kikMode << ";" << kikSize << "\n";

    for (size_t i = 0; i < 3; i++) {

        if (i == 0) {
            coords = parse_coords("../Data/Euc2D/st70.tsp");
            n = 70;
            time = double(n * n * 2000);
        }
        if (i == 1) {
            coords = parse_coords("../Data/Euc2D/a280.tsp");
            n = 280;
            time = double(n * n * 2000);
        }
        if (i == 2) {
            coords = parse_coords("../Data/Euc2D/pr1002.tsp");
            n = 1002;
            time = double(n * n * 2000);
        }

        matrix = coords_to_matrix(coords);

        if (init_rode == 0) roadList = bestStartingNeighbor(n, matrix);
        if (init_rode == 1) roadList = best_random_road(10000, n, matrix);
        if (init_rode == 2) roadList = doNearestNeighbor(n, matrix, 0);

        road = deterministicTabu(roadList, matrix, n, tabuSize, time, enhancementLimit, mode, kikMode, kikSize);

        size_t score = calculate_length(road, matrix, n);

        File << n << ";" << time << ";" << score << "\n";
    }
}

void test_variants_parallel(const std::string& file, int init_rode, int tabuSize, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode)
{
    std::vector<int> road;
    std::vector<int> roadList;
    std::ofstream File(file);
    int** matrix;
    std::vector<std::pair<double, double>> coords;
    size_t n;
    double time;

    File << init_rode << ";" << enhancementLimit << ";" << \
    kickRange.first << ";" << kickRange.second << ";" << mode << "\n";

    for (size_t i = 0; i < 3; i++) {

        if (i == 0) {
            coords = parse_coords("../Data/Euc2D/st70.tsp");
            n = 70;
            time = double(n * n * 2000);
        }
        if (i == 1) {
            coords = parse_coords("../Data/Euc2D/a280.tsp");
            n = 280;
            time = double(n * n * 2000);
        }
        if (i == 2) {
            coords = parse_coords("../Data/Euc2D/pr1002.tsp");
            n = 1002;
            time = double(n * n * 2000);
        }

        matrix = coords_to_matrix(coords);

//        if (init_rode == 0) roadList = bestStartingNeighbor(n, matrix);
//        if (init_rode == 1) roadList = best_random_road(10000, n, matrix);
//        if (init_rode == 2) roadList = doNearestNeighbor(n, matrix, 0);


        for (size_t j = 1; j <= 6; j++){
            road = get_tabu_road_parallel(coords, matrix, n, tabuSize, time, enhancementLimit, kickRange, mode, j);

            size_t score = calculate_length(road, matrix, n);

            File << n << ";" << time << ";" << j << ";" << score << "\n";
        }
    }
}
