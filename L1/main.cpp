#include <iostream>
#include "parser.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "Witaj, Świecie!" << std::endl;

    std::vector<std::pair<double, double>> coords = parse_coords("a280.tsp");
    int** matrix = coords_to_matrix(coords);

    for (size_t i = 0; i < coords.size(); i++) {
        for (size_t j = 0; j < coords.size(); j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
