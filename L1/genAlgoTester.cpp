//
// Created by Franek on 26.05.2022.
//

#include "genAlgoTester.h"
#include "k_random.h"
#include <string.h>
#include <string>


void geneticModeTestSimetric(std::string fileName) {

	std::vector<int> road;
	std::pair<std::vector<int>, int> result;
	int iterations;
	size_t score;

	int** matrix;
	std::vector<std::pair<double, double>> coords;
	coords = parse_coords(fileName);
	matrix = coords_to_matrix(coords);
	int n = coords.size();

	int populationSize = 20;
	double mutationThreshold = 0.05;
	int mutationIntensification = 5;
	double enhanceChance = -1.0;
	double time = 0.25;
	int crossSize = 7;
	int crossCount = populationSize;

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

		std::string testFileName = "Tests/GenTests/sim_" + std::to_string(n) + ".txt";
		std::ofstream File(testFileName, std::ios_base::app);
		File << startMode << ";" << selectionMode << ";" << mutMode << ";" << crossMode << ";" << crossType << ";" << score << ";" << iterations << "\n";

	}}}}}}
}

void geneticModeTestAsimetric(std::string fileName) {

	std::vector<int> road;
	std::pair<std::vector<int>, int> result;
	int iterations;
	size_t score;

	int** matrix;
	std::vector<std::pair<double, double>> coords;
	std::pair<int**, int> res;
	res = parse_matrix(fileName);
	matrix = res.first;
	int n = res.second;

	int populationSize = 20;
	double mutationThreshold = 0.05;
	int mutationIntensification = 5;
	double enhanceChance = -1.0;
	double time = 0.25;
	int crossSize = 7;
	int crossCount = populationSize;

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

		std::string testFileName = "Tests/GenTests/asim_" + std::to_string(n) + ".txt";
		std::ofstream File(testFileName, std::ios_base::app);
		File << startMode << ";" << selectionMode << ";" << mutMode << ";" << crossMode << ";" << crossType << ";" << score << ";" << iterations << "\n";

	}}}}}}

}