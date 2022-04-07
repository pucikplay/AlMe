//
// Created by Gabriel on 26.03.2022.
//

#include "tester.h"
#include <string.h>
#include <string>

#define test_no 3

//Single Algorithm Part
void test_0_K_Rand(std::string fileName) {

	int **matrices;
	std::vector<std::pair<double, double>> coords;

	coords = parse_coords(fileName);
	matrices = coords_to_matrix(coords);

	std::mt19937 rng1(time(nullptr));

	std::vector<int> permutation;
	std::vector<std::pair<size_t, size_t>> bestPermutations;
	size_t best_length = SIZE_MAX;
	size_t length = 0;
	size_t n = coords.size();
	size_t k = 10000;

	if(n < 1000) k *= 2500;
	else if(n < 2000) k*= 1000;
	else if(n < 5000) k*= 250;
	else if(n < 20000) k*= 100;

	//printf("%ld - ", k);

	//k /= 100000;

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < n; i++)
		permutation.push_back(i);

	for (size_t i = 0; i < k; i++) {
		std::shuffle(std::begin(permutation), std::end(permutation), rng1);
		length = calculate_length(permutation, matrices, n);

		if (length < best_length) {
			best_length = length;
			std::pair<size_t, size_t> result;
			result.first = i;
			result.second = best_length;
			bestPermutations.emplace_back(result);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end - start;

	printf("%s\n", fileName.c_str());

	std::size_t pos1 = fileName.rfind('/');
	std::string rawFName = fileName.substr(pos1 + 1);
	pos1 = rawFName.rfind('.');
	rawFName = rawFName.substr(0, pos1);
	rawFName = "Tests/KrandData/" + rawFName + ".txt";

	//std::ofstream File(rawFName);
	std::ofstream File("Tests/krandData.txt", std::ios_base::app);
	for(int i = 0; i < bestPermutations.size(); i++) {
		File << n << ";" << diff.count() << ";" << bestPermutations[i].first << ";" << bestPermutations[i].second << "\n";
	}
}

//Comparison Part
void test_0_0_TSPLIB(const std::string& file) {

	int **matrices[4];
	std::vector<std::pair<double, double>> coords[4];
	int sizes[] = {70, 280, 1002, 2392};
	std::vector<int> road;
	size_t neighbor_len[4];
	size_t random_len[4];
	double times1[4];
	double times2[4];
	std::ofstream File(file);

	coords[0] = parse_coords("Data/Euc2D/st70.tsp");
	coords[1] = parse_coords("Data/Euc2D/a280.tsp");
	coords[2] = parse_coords("Data/Euc2D/pr1002.tsp");
	coords[3] = parse_coords("Data/Euc2D/pr2392.tsp");
	matrices[0] = coords_to_matrix(coords[0]);
	matrices[1] = coords_to_matrix(coords[1]);
	matrices[2] = coords_to_matrix(coords[2]);
	matrices[3] = coords_to_matrix(coords[3]);

	for (size_t i = 0; i < test_no; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		road = best_random_road(10000, sizes[i], matrices[i]);
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff = end - start;
		std::cerr << diff.count() << "\n";
		times1[i] = diff.count();

		neighbor_len[i] = calculate_length(road, matrices[i], sizes[i]);
		start = std::chrono::high_resolution_clock::now();
		road = best_random_road_parallel(10000, sizes[i], matrices[i], 4);
		end = std::chrono::high_resolution_clock::now();

		diff = end - start;
		std::cerr << diff.count() << "\n";
		times2[i] = diff.count();

		random_len[i] = calculate_length(road, matrices[i], sizes[i]);
	}

	for (size_t i = 0; i < test_no; i++) {
		File << sizes[i] << ";" << neighbor_len[i] << ";" << random_len[i] << ";" << times1[i] << ";" << times1[i] << "\n";
	}

}

void test_0_1_TSPLIB(const std::string& file) {

	int **matrices[4];
	std::vector<std::pair<double, double>> coords[4];
	int sizes[] = {70, 280, 1002, 2392};
	std::vector<int> road;
	size_t neighbor_len[4];
	size_t random_len[4];
	double times[4];
	std::ofstream File(file);

	coords[0] = parse_coords("Data/Euc2D/st70.tsp");
	coords[1] = parse_coords("Data/Euc2D/a280.tsp");
	coords[2] = parse_coords("Data/Euc2D/pr1002.tsp");
	coords[3] = parse_coords("Data/Euc2D/pr2392.tsp");
	matrices[0] = coords_to_matrix(coords[0]);
	matrices[1] = coords_to_matrix(coords[1]);
	matrices[2] = coords_to_matrix(coords[2]);
	matrices[3] = coords_to_matrix(coords[3]);

	for (size_t i = 0; i < test_no; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		road = doNearestNeighbor(sizes[i], matrices[i], 0);
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff = end - start;
		std::cerr << diff.count() << "\n";
		times[i] = diff.count();

		neighbor_len[i] = calculate_length(road, matrices[i], sizes[i]);

		road = best_random_road_timed(sizes[i], matrices[i], diff.count());

		random_len[i] = calculate_length(road, matrices[i], sizes[i]);
	}

	for (size_t i = 0; i < test_no; i++) {
		File << sizes[i] << ";" << neighbor_len[i] << ";" << random_len[i] << ";" << times[i] << "\n";
	}

}

void test_0_2_TSPLIB(const std::string& file) {

	int **matrices[4];
	std::vector<std::pair<double, double>> coords[4];
	int sizes[] = {70, 280, 1002, 2392};
	std::vector<int> road;
	size_t _2_opt_len[4];
	size_t random_len[4];
	double times[4];
	std::ofstream File(file);

	coords[0] = parse_coords("Data/Euc2D/st70.tsp");
	coords[1] = parse_coords("Data/Euc2D/a280.tsp");
	coords[2] = parse_coords("Data/Euc2D/pr1002.tsp");
	coords[3] = parse_coords("Data/Euc2D/pr2392.tsp");
	matrices[0] = coords_to_matrix(coords[0]);
	matrices[1] = coords_to_matrix(coords[1]);
	matrices[2] = coords_to_matrix(coords[2]);
	matrices[3] = coords_to_matrix(coords[3]);

	for (size_t i = 0; i < test_no; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		road = get_2_opt_road(best_random_road(1, sizes[i], matrices[i]), matrices[i], sizes[i]);
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff = end - start;
		std::cerr << diff.count() << "\n";
		times[i] = diff.count();

		_2_opt_len[i] = calculate_length(road, matrices[i], sizes[i]);

		road = best_random_road_timed(sizes[i], matrices[i], diff.count());

		random_len[i] = calculate_length(road, matrices[i], sizes[i]);
	}

	for (size_t i = 0; i < test_no; i++) {
		File << sizes[i] << ";" << _2_opt_len[i] << ";" << random_len[i] << ";" << times[i] << "\n";
	}
}

void test_1_2_TSPLIB(const std::string& file) {

	int **matrices[4];
	std::vector<std::pair<double, double>> coords[4];
	int sizes[] = {70, 280, 1002, 2392};
	std::vector<int> road;
	size_t _2_opt_len[4];
	size_t neighbor_len[4];
	double times_neighbor[4];
	double times_2_opt[4];
	std::ofstream File(file);

	coords[0] = parse_coords("Data/Euc2D/st70.tsp");
	coords[1] = parse_coords("Data/Euc2D/a280.tsp");
	coords[2] = parse_coords("Data/Euc2D/pr1002.tsp");
	coords[3] = parse_coords("Data/Euc2D/pr2392.tsp");
	matrices[0] = coords_to_matrix(coords[0]);
	matrices[1] = coords_to_matrix(coords[1]);
	matrices[2] = coords_to_matrix(coords[2]);
	matrices[3] = coords_to_matrix(coords[3]);

	for (size_t i = 0; i < test_no; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		road = doNearestNeighbor(sizes[i], matrices[i], 0);
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff = end - start;
		std::cerr << diff.count() << "\n";
		times_neighbor[i] = diff.count();

		neighbor_len[i] = calculate_length(road, matrices[i], sizes[i]);

		start = std::chrono::high_resolution_clock::now();
		road = get_2_opt_road(road, matrices[i], sizes[i]);
		end = std::chrono::high_resolution_clock::now();

		diff = end - start;
		times_2_opt[i] = diff.count();

		_2_opt_len[i] = calculate_length(road, matrices[i], sizes[i]);
	}

	for (size_t i = 0; i < test_no; i++) {
		File << sizes[i] << ";" << neighbor_len[i] << ";" << times_neighbor[i] << ";" << _2_opt_len[i] << ";" << times_2_opt[i] << "\n";
	}
}

void test_2_2_TSPLIB(const std::string& file) {

	int **matrices[4];
	std::vector<std::pair<double, double>> coords[4];
	int sizes[] = {70, 280, 1002, 2392};
	std::vector<int> road;
	size_t random_len[4];
	size_t neighbor_len[4];
	double times_neighbor[4];
	double times_random[4];
	double part_time;
	std::ofstream File(file);

	coords[0] = parse_coords("Data/Euc2D/st70.tsp");
	coords[1] = parse_coords("Data/Euc2D/a280.tsp");
	coords[2] = parse_coords("Data/Euc2D/pr1002.tsp");
	coords[3] = parse_coords("Data/Euc2D/pr2392.tsp");
	matrices[0] = coords_to_matrix(coords[0]);
	matrices[1] = coords_to_matrix(coords[1]);
	matrices[2] = coords_to_matrix(coords[2]);
	matrices[3] = coords_to_matrix(coords[3]);

	for (size_t i = 0; i < test_no; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		road = doNearestNeighbor(sizes[i], matrices[i], 0);
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff = end - start;
		std::cerr << diff.count() << "\n";

		part_time = diff.count();

		start = std::chrono::high_resolution_clock::now();
		road = get_2_opt_road(road, matrices[i], sizes[i]);;
		end = std::chrono::high_resolution_clock::now();

		diff = end - start;
		times_neighbor[i] = part_time + diff.count();

		neighbor_len[i] = calculate_length(road, matrices[i], sizes[i]);

		start = std::chrono::high_resolution_clock::now();
		road = get_2_opt_road(best_random_road_timed(sizes[i], matrices[i], part_time), matrices[i], sizes[i]);
		end = std::chrono::high_resolution_clock::now();

		diff = end - start;
		times_random[i] = diff.count();

		random_len[i] = calculate_length(road, matrices[i], sizes[i]);
	}

	for (size_t i = 0; i < test_no; i++) {
		File << sizes[i] << ";" << neighbor_len[i] << ";" << times_neighbor[i] << ";" << random_len[i] << ";" << times_random[i] << "\n";
	}
}

void test_Neigh_TSPLIB(const std::string& file) {

	int **matrices[4];
	std::vector<std::pair<double, double>> coords[4];
	int sizes[] = {70, 280, 1002, 2392};
	std::vector<int> road;
	size_t neighbor_len[4];
	size_t best_neighbor_len[4];
	size_t branch_neighbor_len[4];
	size_t best_branch_neighbor_len[4];
	size_t random_len[4];
	double times1[4];
	double times2[4];
	double times3[4];
	double times4[4];
	std::ofstream File(file);

	coords[0] = parse_coords("Data/Euc2D/st70.tsp");
	coords[1] = parse_coords("Data/Euc2D/a280.tsp");
	coords[2] = parse_coords("Data/Euc2D/pr1002.tsp");
	coords[3] = parse_coords("Data/Euc2D/pr2392.tsp");
	matrices[0] = coords_to_matrix(coords[0]);
	matrices[1] = coords_to_matrix(coords[1]);
	matrices[2] = coords_to_matrix(coords[2]);
	matrices[3] = coords_to_matrix(coords[3]);

	for (size_t i = 0; i < test_no; i++) {
		auto start1 = std::chrono::high_resolution_clock::now();
		road = doNearestNeighbor(sizes[i], matrices[i], 0);
		auto end1 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff1 = end1 - start1;
		std::cerr << diff1.count() << "\n";
		times1[i] = diff1.count();

		neighbor_len[i] = calculate_length(road, matrices[i], sizes[i]);

		auto start2 = std::chrono::high_resolution_clock::now();
		road = bestStartingNeighbor(sizes[i], matrices[i]);
		auto end2 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff2 = end2 - start2;
		std::cerr << diff2.count() << "\n";
		times2[i] = diff2.count();

		best_neighbor_len[i] = calculate_length(road, matrices[i], sizes[i]);

		auto start3 = std::chrono::high_resolution_clock::now();
		road = doBranchingNeighbor(sizes[i], matrices[i], 0, 2);
		auto end3 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff3 = end3 - start3;
		std::cerr << diff3.count() << "\n";
		times3[i] = diff3.count();

		branch_neighbor_len[i] = calculate_length(road, matrices[i], sizes[i]);

		auto start4 = std::chrono::high_resolution_clock::now();
		road = bestBranchingNeighbor(sizes[i], matrices[i], 2);
		auto end4 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff4 = end4 - start4;
		std::cerr << diff4.count() << "\n";
		times4[i] = diff4.count();

		best_branch_neighbor_len[i] = calculate_length(road, matrices[i], sizes[i]);
	}

	for (size_t i = 0; i < test_no; i++) {
		File << sizes[i] << ";" << neighbor_len[i] << ";" << times1[i] << ";" << best_neighbor_len[i] << ";" << times2[i] << ";" << branch_neighbor_len[i] << ";" << times3[i] << ";" << best_branch_neighbor_len[i] << ";" << times4[i] << "\n";
	}

}

//Mass Comparison Part
void mass_test_0_1_TSPLIB(std::string fileName) {

	int **matrix;
	std::vector<std::pair<double, double>> coords;
	std::vector<int> road;

	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	if(n > 2000) return;

	auto start = std::chrono::high_resolution_clock::now();
	road = doNearestNeighbor(n, matrix, 0);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end - start;
	double times = diff.count();

	size_t neighbor_len = calculate_length(road, matrix, n);
	road = best_random_road_timed(n, matrix, diff.count());
	size_t random_len = calculate_length(road, matrix, n);

	std::ofstream File("Tests/mass01test.txt", std::ios_base::app);
	File << n << ";" << neighbor_len << ";" << random_len << ";" << times << "\n";
}

void mass_test_0_2_TSPLIB(std::string fileName) {

	int **matrix;
	std::vector<std::pair<double, double>> coords;
	std::vector<int> road;

	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	if(n > 2000) return;

	auto start = std::chrono::high_resolution_clock::now();
	road = get_2_opt_road(best_random_road(1, n, matrix), matrix, n);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end - start;
	double times = diff.count();

	size_t _2_opt_len = calculate_length(road, matrix, n);
	road = best_random_road_timed(n, matrix, times);
	size_t random_len = calculate_length(road, matrix, n);

	std::ofstream File("Tests/mass02test.txt", std::ios_base::app);
	File << n << ";" << _2_opt_len << ";" << random_len << ";" << times << "\n";
}

void mass_test_1_2_TSPLIB(std::string fileName) {

	int **matrix;
	std::vector<std::pair<double, double>> coords;
	std::vector<int> road;

	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	if(n > 2000) return;

	auto start = std::chrono::high_resolution_clock::now();
	road = doNearestNeighbor(n, matrix, 0);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end - start;
	double times_neighbor = diff.count();
	size_t neighbor_len = calculate_length(road, matrix, n);

	start = std::chrono::high_resolution_clock::now();
	road = get_2_opt_road(road, matrix, n);
	end = std::chrono::high_resolution_clock::now();
	
	diff = end - start;
	double times_2_opt = diff.count();
	size_t _2_opt_len = calculate_length(road, matrix, n);

	std::ofstream File("Tests/mass12test.txt", std::ios_base::app);
	File << n << ";" << neighbor_len << ";" << times_neighbor << ";" << _2_opt_len << ";" << times_2_opt << "\n";
}

void mass_test_02_12_TSPLIB(std::string fileName) {

	int **matrix;
	std::vector<std::pair<double, double>> coords;
	std::vector<int> road1, road2, roadOpt1, roadOpt2;

	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	std::chrono::duration<double> diff;

	if(n > 2000) return;

	auto start = std::chrono::high_resolution_clock::now();
	road1 = doNearestNeighbor(n, matrix, 0);
	auto middle = std::chrono::high_resolution_clock::now();
	roadOpt1 = get_2_opt_road(road1, matrix, n);
	auto end = std::chrono::high_resolution_clock::now();
	
	diff = middle - start;
	double times = diff.count();
	diff = end - middle;
	double timesNeiOpt = diff.count();

	road2 = best_random_road_timed(n, matrix, diff.count());
	middle = std::chrono::high_resolution_clock::now();
	roadOpt2 = get_2_opt_road(road2, matrix, n);
	end = std::chrono::high_resolution_clock::now();

	diff = end - middle;
	double timesRandOpt = diff.count();

	size_t neighborStart = calculate_length(road1, matrix, n);
	size_t neighbor2Opt = calculate_length(roadOpt1, matrix, n);
	size_t krandStart = calculate_length(road2, matrix, n);
	size_t krand2Opt = calculate_length(roadOpt2, matrix, n);

	std::ofstream File("Tests/mass22test.txt", std::ios_base::app);
	File << n << ";" << neighborStart << ";" << neighbor2Opt << ";" << timesNeiOpt << ";" << krandStart << ";" << krand2Opt << ";" << timesRandOpt << "\n";
}

void mass_test_2_3_TSPLIB(std::string fileName) {

	int **matrix;
	std::vector<std::pair<double, double>> coords;
	std::vector<int> road, roadOpt1, roadOpt2;

	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	std::chrono::duration<double> diff;

	if(n > 500) return;

	road = bestStartingNeighbor(n, matrix);

	auto middle = std::chrono::high_resolution_clock::now();
	roadOpt1 = get_2_opt_road(road, matrix, n);
	auto end = std::chrono::high_resolution_clock::now();

	diff = end - middle;
	double times2Opt = diff.count();

	middle = std::chrono::high_resolution_clock::now();
	roadOpt2 = get_3_opt_road(road, matrix, n);
	end = std::chrono::high_resolution_clock::now();

	diff = end - middle;
	double times3Opt = diff.count();

	size_t algStart = calculate_length(road, matrix, n);
	size_t result2Opt = calculate_length(roadOpt1, matrix, n);
	size_t result3Opt = calculate_length(roadOpt2, matrix, n);

	std::ofstream File("Tests/mass23test.txt", std::ios_base::app);
	File << n << ";" << algStart << ";" << result2Opt << ";" << times2Opt << ";" << result3Opt << ";" << times3Opt << "\n";
}

void mass_test_breachDepth_TSPLIB(std::string fileName) {

	int **matrix;
	std::vector<std::pair<double, double>> coords;
	std::vector<int> road, roadOpt;

	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	std::chrono::duration<double> diff;

	if(n > 5000) return;
	for(int i = 0; i <= 24; i++) {

		auto start = std::chrono::high_resolution_clock::now();
		road = bestBranchingNeighbor(coords.size(), matrix, i);
		auto middle = std::chrono::high_resolution_clock::now();
		roadOpt = get_2_opt_road(road, matrix, n);
		auto end = std::chrono::high_resolution_clock::now();

		diff = middle - start;
		double timesBranch = diff.count();

		diff = end - middle;
		double times2Opt = diff.count();

		size_t branchStart = calculate_length(road, matrix, n);
		size_t result2Opt = calculate_length(roadOpt, matrix, n);

		std::ofstream File("Tests/massBranchTest.txt", std::ios_base::app);
		File << n << ";" << i << ";" << branchStart << ";" << result2Opt << ";" << timesBranch << ";" << times2Opt << "\n";
	}
}

void mass_test_Neigh_TSPLIB(std::string fileName) {

	int **matrix;
	std::vector<std::pair<double, double>> coords;
	std::vector<int> road;

	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	if(n > 2000) return;

	std::ofstream File("Tests/massNeightest.txt", std::ios_base::app);

	//Single Nearest
	auto start1 = std::chrono::high_resolution_clock::now();
	road = doNearestNeighbor(n, matrix, 0);
	auto end1 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff1 = end1 - start1;
	double times1 = diff1.count();
	size_t neighbor_len = calculate_length(road, matrix, n);

	//Testing every beginning
	auto start2 = std::chrono::high_resolution_clock::now();
	road = bestStartingNeighbor(n, matrix);
	auto end2 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff2 = end2 - start2;
	double times2 = diff2.count();
	size_t best_neighbor_len = calculate_length(road, matrix, n);

	//Single Branching
	auto start3 = std::chrono::high_resolution_clock::now();
	road = doBranchingNeighbor(n, matrix, 0, 2);
	auto end3 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff3 = end3 - start3;
	double times3 = diff3.count();
	size_t branch_neighbor_len = calculate_length(road, matrix, n);

	//Testing every beginning
	auto start4 = std::chrono::high_resolution_clock::now();
	road = bestBranchingNeighbor(n, matrix, 2);
	auto end4 = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff4 = end4 - start4;
	double times4 = diff4.count();
	size_t best_branch_neighbor_len = calculate_length(road, matrix, n);

	File << n << ";" << neighbor_len << ";" << times1 << ";" << best_neighbor_len << ";" << times2 << ";" << branch_neighbor_len << ";" << times3 << ";" << best_branch_neighbor_len << ";" << times4 << "\n";
}

void mass_k_random_parallel(std::string fileName) {
    int **matrix;
    std::vector<std::pair<double, double>> coords;
    std::vector<int> road;
    std::chrono::time_point<std::chrono::system_clock> starts[7];
    std::chrono::time_point<std::chrono::system_clock> ends[7];
    size_t lengths[7];

    coords = parse_coords(fileName);
    matrix = coords_to_matrix(coords);
    int n = coords.size();

    for (size_t i = 6; i > 1; i--) {
        starts[i] = std::chrono::high_resolution_clock::now();
        road = best_random_road_parallel(10000, n, matrix, i);
        ends[i] = std::chrono::high_resolution_clock::now();
        lengths[i] = calculate_length(road, matrix, n);
    }

    starts[1] = std::chrono::high_resolution_clock::now();
    road = best_random_road(10000, n, matrix);
    ends[1] = std::chrono::high_resolution_clock::now();
    lengths[1] = calculate_length(road, matrix, n);

    std::ofstream File("../Tests/massParallelTest.txt", std::ios_base::app);

    File << n;

    for (size_t i = 1; i < 7; i++) {
        File << ";" << lengths[i];
    }

    for (size_t i = 1; i < 7; i++) {
        File << ";" << (ends[i] - starts[i]).count();
    }

    File << "\n";

}

void mass_neighbor_parallel(std::string fileName) {
    int **matrix;
    std::vector<std::pair<double, double>> coords;
    std::vector<int> road;
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;
    size_t length;

    coords = parse_coords(fileName);
    matrix = coords_to_matrix(coords);
    int n = coords.size();

    start = std::chrono::high_resolution_clock::now();
    road = doBranchingNeighborParallel(n, matrix, 0, 15, 6);
    end = std::chrono::high_resolution_clock::now();
    length = calculate_length(road, matrix, n);

    std::ofstream File("../Tests/massNeighborParallelTest.txt", std::ios_base::app);

    File << n << ";" << length << ";" << (end - start).count() << "\n";

}