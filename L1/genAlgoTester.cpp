//
// Created by Franek on 26.05.2022.
//

#include "genAlgoTester.h"
#include "k_random.h"
#include <string.h>
#include <string>

std::mt19937 rngPar(time(nullptr));

// Testing modes
// 7.2H / File
void geneticModeTest(std::string fileName, int n, int** matrix, int mode) {

	std::vector<int> road;
	std::pair<std::vector<int>, int> result;
	int iterations;
	size_t score;

	int populationSize = 20;
	double mutationThreshold = 0.05;
	int mutationIntensification = 5;
	double enhanceChance = -1.0;
	double time = 30.0;
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
	for(int i = 0; i < 4; i++) {

		result = geneticMainTimed(n, matrix, populationSize, mutationThreshold, mutMode, mutationIntensification, enhanceChance, time, crossMode, crossSize, crossType, crossCount, selectionMode, startMode);
		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File << counter / 4 << ";" << startMode << ";" << selectionMode << ";" << mutMode << ";" << crossMode << ";" << crossType << ";" << score << ";" << iterations << "\n";
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
// 6.66H / File
void geneticParseeker(std::string fileName, int n, int** matrix, int mode) {

	std::vector<int> road;
	std::pair<std::vector<int>, int> result;
	int iterations;
	size_t score;

	// Filled with data from Mode test
	int startMode = 1;
	int selectionMode = 0;
	int mutMode = 0;
	int crossMode = 1;
	int crossType = 1;

	double enhanceChance = -1.0;
	double time = 30.0;

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
		populationSize = 10 + rngPar() % 91;
		mutationThreshold = unif(rngPar);
		mutationIntensification = 1 + rngPar() % 20;
		crossSize = 2 + rngPar() % 18;
		crossCount = 10 + rngPar() % 190;
	for(int i = 0; i < 4; i++) {

		result = geneticMainTimed(n, matrix, populationSize, mutationThreshold, mutMode, mutationIntensification, enhanceChance, time, crossMode, crossSize, crossType, crossCount, selectionMode, startMode);
		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File << counter / 4 << ";" << populationSize << ";" << mutationThreshold << ";" << mutationIntensification << ";" << crossSize << ";" << crossCount << ";" << score << ";" << iterations << "\n";
		counter += 1;
	}}

	// second dataset - specific for simetry
	if(mode == 0) {
		startMode = 1;
		selectionMode = 0;
		mutMode = 3;
		crossMode = 2;
		crossType = 2;
		testFileName = "Tests/GenTests/par2_sim_" + std::to_string(n) + ".txt";
	} else if(mode == 1) {
		startMode = 1;
		selectionMode = 0;
		mutMode = 0;
		crossMode = 1;
		crossType = 2;
		testFileName = "Tests/GenTests/par2_asim_" + std::to_string(n) + ".txt";
	}
	std::ofstream File2(testFileName, std::ios_base::app);

	counter = 0;

	for(int j = 0; j < 100; j++) {
		populationSize = 10 + rngPar() % 91;
		mutationThreshold = unif(rngPar);
		mutationIntensification = 1 + rngPar() % 20;
		crossSize = 2 + rngPar() % 18;
		crossCount = 10 + rngPar() % 190;
	for(int i = 0; i < 4; i++) {

		result = geneticMainTimed(n, matrix, populationSize, mutationThreshold, mutMode, mutationIntensification, enhanceChance, time, crossMode, crossSize, crossType, crossCount, selectionMode, startMode);
		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File2 << counter / 4 << ";" << populationSize << ";" << mutationThreshold << ";" << mutationIntensification << ";" << crossSize << ";" << crossCount << ";" << score << ";" << iterations << "\n";
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
void geneticAgeTester(std::string fileName, int n, int** matrix, int mode) {

	std::vector<int> road;
	std::pair<std::vector<int>, int> result;
	int iterations;
	size_t score;

	double time = 30.0;

	std::string testFileName;
	if(mode == 0) testFileName = "Tests/GenTests/age_sim_" + std::to_string(n) + ".txt";
	else if(mode == 1) testFileName = "Tests/GenTests/age_asim_" + std::to_string(n) + ".txt";
	std::ofstream File(testFileName, std::ios_base::app);

	int counter = 0;

	for(int ageMax = 1; ageMax <= 20; ageMax++) {
	for(int i = 0; i < 6; i++) {
			
		if(n == 70) result = geneticMainTimed(n, matrix, 39, 0.370, 3, 13, 0.01, time, 2, 19, 2, 177, 0, 1, ageMax);
		else if(n == 439) result = geneticMainTimed(n, matrix, 51, 0.913, 0, 1, 0.01, time, 1, 11, 1, 181, 0, 1, ageMax);
		else if(n == 71) result = geneticMainTimed(n, matrix, 91, 0.067, 0, 18, 0.01, time, 1, 17, 2, 177, 0, 1, ageMax);
		else if(n == 443) result = geneticMainTimed(n, matrix, 91, 0.993, 0, 19, 0.01, time, 1, 12, 2, 176, 0, 1, ageMax);


		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File << counter / 6 << ";" << ageMax << ";" << score << ";" << iterations << "\n";
		counter += 1;
	}}
}

void geneticAgeTesterSimetric(std::string fileName) {

	int** matrix;
	std::vector<std::pair<double, double>> coords;
	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	geneticAgeTester(fileName, n, matrix, 0);
}

void geneticAgeTesterAsimetric(std::string fileName) {
	int** matrix;
	std::vector<std::pair<double, double>> coords;
	std::pair<int**, int> res;
	res = parse_matrix(fileName);
	matrix = res.first;
	int n = res.second;

	geneticAgeTester(fileName, n, matrix, 1);
}

// Testing Local Enhancement
// 4.4H / File
void geneticLocEnhaTester(std::string fileName, int n, int** matrix, int mode) {

	std::vector<int> road;
	std::pair<std::vector<int>, int> result;
	int iterations;
	size_t score;

	double time = 30.0;

	std::string testFileName;
	if(mode == 0) testFileName = "Tests/GenTests/le_sim_" + std::to_string(n) + ".txt";
	else if(mode == 1) testFileName = "Tests/GenTests/le_asim_" + std::to_string(n) + ".txt";
	std::ofstream File(testFileName, std::ios_base::app);

	int counter = 0;

	for(double enhanceChance = 0.025; enhanceChance <= 1.020; enhanceChance += 0.025) {
	for(int i = 0; i < 4; i++) {
			
		if(n == 70) result = geneticMainTimed(n, matrix, 39, 0.370, 3, 13, enhanceChance, time, 2, 19, 2, 177, 0, 1, -1);
		else if(n == 439) result = geneticMainTimed(n, matrix, 51, 0.913, 0, 1, enhanceChance, time, 1, 11, 1, 181, 0, 1, -1);
		else if(n == 71) result = geneticMainTimed(n, matrix, 91, 0.067, 0, 18, enhanceChance, time, 1, 17, 2, 177, 0, 1, -1);
		else if(n == 443) result = geneticMainTimed(n, matrix, 91, 0.993, 0, 19, enhanceChance, time, 1, 12, 2, 176, 0, 1, -1);

		road = result.first;
		iterations = result.second;
		score = calculate_length(road, matrix, road.size());

		File << counter / 4 << ";" << enhanceChance << ";" << score << ";" << iterations << "\n";
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
// 1 min / File

// Testing Islands
