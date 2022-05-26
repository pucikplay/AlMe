//
// Created by Franek on 26.05.2022.
//

#include "genAlgoTester.h"
#include "k_random.h"
#include <string.h>
#include <string>

std::mt19937 rngPar(time(nullptr));

// Testing modes
void geneticModeTest(std::string fileName, int n, int** matrix, int mode) {

	std::vector<int> road;
	std::pair<std::vector<int>, int> result;
	int iterations;
	size_t score;

	int populationSize = 20;
	double mutationThreshold = 0.05;
	int mutationIntensification = 5;
	double enhanceChance = -1.0;
	double time = 0.25;
	int crossSize = 7;
	int crossCount = populationSize;

	std::string testFileName;
	if(mode == 0) testFileName = "Tests/GenTests/sim_" + std::to_string(n) + ".txt";
	else if(mode == 1) testFileName = "Tests/GenTests/asim_" + std::to_string(n) + ".txt";
	std::ofstream File(testFileName, std::ios_base::app);

	int counter = 0;

	for(int startMode = 0; startMode < 3; startMode += 1) {
	for(int selectionMode = 0; selectionMode < 2; selectionMode += 1) {
	for(int mutMode = 0; mutMode < 4; mutMode += 1) {
	for(int crossMode = 0; crossMode < 3; crossMode += 1) {
	for(int crossType = 0; crossType < 3; crossType += 1) {
	for(int i = 0; i < 3; i++) {

		result = geneticMainTimed(n, matrix, populationSize, mutationThreshold, mutMode, mutationIntensification, enhanceChance, time, crossMode, crossSize, crossType, crossCount, selectionMode, startMode);
		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File << counter / 3 << ";" << startMode << ";" << selectionMode << ";" << mutMode << ";" << crossMode << ";" << crossType << ";" << score << ";" << iterations << "\n";
		counter += 1;
	}}}}}}

}

void geneticModeTestSimetric(std::string fileName) {

	int** matrix;
	std::vector<std::pair<double, double>> coords;
	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	geneticModeTest(fileName, n, matrix, 0);
}

void geneticModeTestAsimetric(std::string fileName) {

	int** matrix;
	std::vector<std::pair<double, double>> coords;
	std::pair<int**, int> res;
	res = parse_matrix(fileName);
	matrix = res.first;
	int n = res.second;

	geneticModeTest(fileName, n, matrix, 1);
}

// Seeking for parameters
void geneticParseeker(std::string fileName, int n, int** matrix, int mode) {

	std::vector<int> road;
	std::pair<std::vector<int>, int> result;
	int iterations;
	size_t score;

	// Filled with data from Mode test
	int startMode = 0;
	int selectionMode = 0;
	int mutMode = 0;
	int crossMode = 0;
	int crossType = 0;

	double enhanceChance = -1.0;
	double time = 0.25;

	// will be random at each iteration
	int populationSize = 20;
	double mutationThreshold = 0.05;
	int mutationIntensification = 5;
	int crossSize = 7;
	int crossCount = populationSize;

	std::uniform_real_distribution<double> unif(0.0, 1.0);
	std::string testFileName;
	if(mode == 0) testFileName = "Tests/GenTests/par1_sim_" + std::to_string(n) + ".txt";
	else if(mode == 1) testFileName = "Tests/GenTests/par1_asim_" + std::to_string(n) + ".txt";
	std::ofstream File(testFileName, std::ios_base::app);

	int counter = 0;

	for(int j = 0; j < 100; j++) {
		populationSize = 10 + rngPar() % 90;
		mutationThreshold = unif(rngPar);
		mutationIntensification = 1 + rngPar() % 10;
		crossSize = 2 + rngPar() % 15;
		crossCount = 10 + rngPar() % 190;
	for(int i = 0; i < 3; i++) {

		result = geneticMainTimed(n, matrix, populationSize, mutationThreshold, mutMode, mutationIntensification, enhanceChance, time, crossMode, crossSize, crossType, crossCount, selectionMode, startMode);
		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File << counter / 3 << ";" << populationSize << ";" << mutationThreshold << ";" << mutationIntensification << ";" << crossSize << ";" << crossCount << ";" << score << ";" << iterations << "\n";
		counter += 1;
	}}

	// second dataset - specific for simetry
	if(mode == 0) {
		startMode = 0;
		selectionMode = 0;
		mutMode = 0;
		crossMode = 0;
		crossType = 0;
		testFileName = "Tests/GenTests/par2_sim_" + std::to_string(n) + ".txt";
	} else if(mode == 1) {
		startMode = 0;
		selectionMode = 0;
		mutMode = 0;
		crossMode = 0;
		crossType = 0;
		testFileName = "Tests/GenTests/par2_asim_" + std::to_string(n) + ".txt";
	}
	std::ofstream File2(testFileName, std::ios_base::app);

	counter = 0;

	for(int j = 0; j < 100; j++) {
		populationSize = 10 + rngPar() % 90;
		mutationThreshold = unif(rngPar);
		mutationIntensification = 1 + rngPar() % 10;
		crossSize = 2 + rngPar() % 15;
		crossCount = 10 + rngPar() % 190;
	for(int i = 0; i < 3; i++) {

		result = geneticMainTimed(n, matrix, populationSize, mutationThreshold, mutMode, mutationIntensification, enhanceChance, time, crossMode, crossSize, crossType, crossCount, selectionMode, startMode);
		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File2 << counter / 3 << ";" << populationSize << ";" << mutationThreshold << ";" << mutationIntensification << ";" << crossSize << ";" << crossCount << ";" << score << ";" << iterations << "\n";
		counter += 1;
	}}
}

void geneticParseekerSimetric(std::string fileName) {

	int** matrix;
	std::vector<std::pair<double, double>> coords;
	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	geneticParseeker(fileName, n, matrix, 0);
}

void geneticParseekerAsimetric(std::string fileName) {
	int** matrix;
	std::vector<std::pair<double, double>> coords;
	std::pair<int**, int> res;
	res = parse_matrix(fileName);
	matrix = res.first;
	int n = res.second;

	geneticParseeker(fileName, n, matrix, 1);
}

// Testing age

// Testing Local Enhancement
void geneticLocEnhaTester(std::string fileName, int n, int** matrix, int mode) {

	std::vector<int> road;
	std::pair<std::vector<int>, int> result;
	int iterations;
	size_t score;

	double time = 0.25;

	//filled with data from mode tester and parseeker
	int startMode = 0;
	int selectionMode = 0;
	int mutMode = 0;
	int crossMode = 0;
	int crossType = 0;

	int populationSize = 20;
	double mutationThreshold = 0.05;
	int mutationIntensification = 5;
	int crossSize = 7;
	int crossCount = populationSize;

	std::uniform_real_distribution<double> unif(0.0, 1.0);
	std::string testFileName;
	if(mode == 0) testFileName = "Tests/GenTests/le1_sim_" + std::to_string(n) + ".txt";
	else if(mode == 1) testFileName = "Tests/GenTests/le1_asim_" + std::to_string(n) + ".txt";
	std::ofstream File(testFileName, std::ios_base::app);

	int counter = 0;

	for(double enhanceChance = 0.025; enhanceChance <= 1.0; enhanceChance += 0.025) {
	for(int i = 0; i < 5; i++) {

		result = geneticMainTimed(n, matrix, populationSize, mutationThreshold, mutMode, mutationIntensification, enhanceChance, time, crossMode, crossSize, crossType, crossCount, selectionMode, startMode);
		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File << counter / 5 << ";" << enhanceChance << ";" << score << ";" << iterations << "\n";
		counter += 1;
	}}

	// second dataset
	if(mode == 0) {
		startMode = 0;
		selectionMode = 0;
		mutMode = 0;
		crossMode = 0;
		crossType = 0;

		populationSize = 20;
		mutationThreshold = 0.05;
		mutationIntensification = 5;
		crossSize = 7;
		crossCount = populationSize;

		testFileName = "Tests/GenTests/le2_sim_" + std::to_string(n) + ".txt";

	} else if(mode == 1) {
		startMode = 0;
		selectionMode = 0;
		mutMode = 0;
		crossMode = 0;
		crossType = 0;

		populationSize = 20;
		mutationThreshold = 0.05;
		mutationIntensification = 5;
		crossSize = 7;
		crossCount = populationSize;

		testFileName = "Tests/GenTests/le2_asim_" + std::to_string(n) + ".txt";
	}

	std::ofstream File2(testFileName, std::ios_base::app);

	counter = 0;

	for(double enhanceChance = 0.025; enhanceChance <= 1.0; enhanceChance += 0.025) {
	for(int i = 0; i < 5; i++) {

		result = geneticMainTimed(n, matrix, populationSize, mutationThreshold, mutMode, mutationIntensification, enhanceChance, time, crossMode, crossSize, crossType, crossCount, selectionMode, startMode);
		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File2 << counter / 5 << ";" << enhanceChance << ";" << score << ";" << iterations << "\n";
		counter += 1;
	}}
}

void geneticLocEnhaTesterSimetric(std::string fileName) {

	int** matrix;
	std::vector<std::pair<double, double>> coords;
	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	geneticLocEnhaTester(fileName, n, matrix, 0);
}

void geneticLocEnhaTesterAsimetric(std::string fileName) {
	int** matrix;
	std::vector<std::pair<double, double>> coords;
	std::pair<int**, int> res;
	res = parse_matrix(fileName);
	matrix = res.first;
	int n = res.second;

	geneticLocEnhaTester(fileName, n, matrix, 1);
}

// Getting Tabu Data

// Testing Islands
