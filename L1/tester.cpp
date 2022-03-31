//
// Created by Gabriel on 26.03.2022.
//

#include "tester.h"

#define test_no 3

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