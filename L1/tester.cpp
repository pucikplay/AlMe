//
// Created by Gabriel on 26.03.2022.
//

#include "tester.h"

#define test_no 3

void test_0_1_TSPLIB(const std::string& file) {

    int **matrices[4];
    std::vector<std::pair<double, double>> coords[4];
    int sizes[] = {70, 280, 1002, 2392};
    std::vector<int> road;
    size_t neighbor_len[4];
    size_t random_len[4];
    double times[4];
    std::ofstream File(file);

    coords[0] = parse_coords("../Data/Euc2D/st70.tsp");
    coords[1] = parse_coords("../Data/Euc2D/a280.tsp");
    coords[2] = parse_coords("../Data/Euc2D/pr1002.tsp");
    coords[3] = parse_coords("../Data/Euc2D/pr2392.tsp");
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

    coords[0] = parse_coords("../Data/Euc2D/st70.tsp");
    coords[1] = parse_coords("../Data/Euc2D/a280.tsp");
    coords[2] = parse_coords("../Data/Euc2D/pr1002.tsp");
    coords[3] = parse_coords("../Data/Euc2D/pr2392.tsp");
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

    coords[0] = parse_coords("../Data/Euc2D/st70.tsp");
    coords[1] = parse_coords("../Data/Euc2D/a280.tsp");
    coords[2] = parse_coords("../Data/Euc2D/pr1002.tsp");
    coords[3] = parse_coords("../Data/Euc2D/pr2392.tsp");
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

    coords[0] = parse_coords("../Data/Euc2D/st70.tsp");
    coords[1] = parse_coords("../Data/Euc2D/a280.tsp");
    coords[2] = parse_coords("../Data/Euc2D/pr1002.tsp");
    coords[3] = parse_coords("../Data/Euc2D/pr2392.tsp");
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
