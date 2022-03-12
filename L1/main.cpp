#include <iostream>
#include "parser.h"
#include "visualizer.h"

int main() {

	//doSimpleVisual();

	std::cout << "Hello, World!" << std::endl;
	std::cout << "Witaj, Świecie!" << std::endl;

	std::vector<std::pair<double, double>> coords = parse_coords("a280.tsp");
	int** matrix = coords_to_matrix(coords);

	doEucDraw(coords, matrix);

	int counter = 0;
	for (size_t i = 0; i < coords.size(); i++) {
		for (size_t j = 0; j < coords.size(); j++) {
			counter += 1;
			//printf("%d ", matrix[i][j]);
		}
		//printf("\n");
	}

	printf("%d\n", counter);

	return 0;
}
