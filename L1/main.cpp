#include <iostream>
#include "parser.h"
#include "visualizer.h"
#include "random_generator.h"

int main(int argc, char *argv[]) {

	//doSimpleVisual();

	std::cout << "Hello, World!" << std::endl;
	std::cout << "Witaj, Świecie!" << std::endl;

//	std::vector<std::pair<double, double>> coords = parse_coords("a280.tsp");
//	int** matrix = coords_to_matrix(coords);

	//doEucDraw(coords, matrix);

//	int counter = 0;
//	for (size_t i = 0; i < coords.size(); i++) {
//		for (size_t j = 0; j < coords.size(); j++) {
//			counter += 1;
//			printf("%d ", matrix[i][j]);
//		}
//		printf("\n");
//	}

//	printf("%d\n", counter);

    size_t n = 100;
    std::vector<std::pair<double, double>> coords = gen_coords(n);

    for (size_t i = 0; i < n; i++) {
        printf("%d %d %d\n", i, coords[i].first, coords[i].second);
    }

	return 0;
}
