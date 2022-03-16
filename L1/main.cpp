#include <iostream>
#include "parser.h"
#include "visualizer.h"
#include "random_generator.h"

int main(int argc, char *argv[]) {

	std::cout << "Hello, Meta!" << std::endl;

	//Euclidean

	//Generated
	//size_t n = 100;
	//std::vector<std::pair<double, double>> coords = gen_coords(n);

	//Euc-2D
	//std::vector<std::pair<double, double>> coords = parse_coords("Data/Euc2D/a280.tsp");
	//std::vector<std::pair<double, double>> coords = parse_coords("Data/Euc2D/st70.tsp");

	//int** matrix = coords_to_matrix(coords);
	//doEucDraw(coords, matrix);


	//Matrix
	std::pair<int**, int> result;

	//Full Matrix
	//result = parse_matrix("Data/FullMatrix/swiss42.tsp");
	//result = parse_matrix("Data/FullMatrix/bays29.tsp");

	//Upper Row
	//result = parse_matrix("Data/UpperRow/brg180.tsp");
	//result = parse_matrix("Data/UpperRow/brazil58.tsp");
	result = parse_matrix("Data/UpperRow/bayg29.tsp");

	int** matrix = result.first;
	int n = result.second;
	doMatrixDraw(matrix, n);

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
