//
// Created by Gabriel on 11.03.2022.
//

#include <fstream>
#include <sstream>
#include <math.h>
#include <iostream>
#include "parser.h"

int distance(std::pair<double, double> p1, std::pair<double, double> p2) {
	double dist = sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
	return (int) (dist + 0.5);
}

std::vector<std::pair<double, double>> parse_coords(const std::string& file) {
	std::ifstream File(file);
	std::string line;
	std::vector<std::pair<double, double>> list;
	int i;
	double n, m;

	while (std::getline(File, line) && line.compare("NODE_COORD_SECTION"));

	while (File >> i >> n >> m) {
		list.emplace_back(n, m);
	}
	File.close();

	return list;
}

int** coords_to_matrix(std::vector<std::pair<double, double>> coords) {
	int** matrix = 0;
	size_t n = coords.size();
	matrix = new int* [n];

	for (size_t i = 0; i < n; i ++) {
		matrix[i] = new int[n];
	}

	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < n; j++) {
			matrix[i][j] = -1;
		}
	}

	for (size_t i = 0; i < n; i++) {
		for (size_t j = i + 1; j < n; j++) {
			matrix[i][j] = distance(coords[i], coords[j]);
		}
	}
	return matrix;
}

std::pair<int**, int> parse_matrix(const std::string& file) {
	int** matrix = 0;
	std::ifstream File(file);
	std::string line;
	int n, tmp;

	while (std::getline(File, line) && line.find("DIMENSION"));
	std::string dim = line.substr(11);
	n = std::stoi(dim);

	matrix = new int* [n];
	for (size_t i = 0; i < n; i ++) {
		matrix[i] = new int[n];
	}

	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < n; j++) {
			matrix[i][j] = -1;
		}
	}

	while (std::getline(File, line) && line.find("EDGE_WEIGHT_FORMAT"));
	std::string format = line.substr(20);

	std::pair<int**, int>  result;

	if (format.find("UPPER_ROW")) {
		while (std::getline(File, line) && line.find("EDGE_WEIGHT_SECTION"));
		for (size_t i = 0; i < n; i++) {
			for (size_t j = i + 1; j < n; j++) {
				File >> matrix[i][j];
			}
		}
		File.close();

		result.first = matrix;
		result.second = n;
		return result;

	} else if (format.find("LOWER_ROW")) {
		while (std::getline(File, line) && line.find("EDGE_WEIGHT_SECTION"));

	} else if (format.find("FULL_MATRIX")) {
		while (std::getline(File, line) && line.find("EDGE_WEIGHT_SECTION"));

		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < n; j++) {
				File >> matrix[i][j];
			}
		}
		File.close();

		result.first = matrix;
		result.second = n;
		return result;

	}
	result.first = matrix;
	result.second = n;
	return result;
}
