//
// Created by Gabriel on 26.03.2022.
//

#ifndef ALME_TESTER_H
#define ALME_TESTER_H

#include "parser.h"
#include "2_opt.h"
#include "roadSolver.h"
#include "k_random.h"
#include <vector>
#include <ctime>
#include <fstream>
#include <cstring>
#include <iostream>
#include <chrono>

//Single Algorithm Part
void test_0_K_Rand(std::string fileName);

//Comparison Part
void test_0_0_TSPLIB(const std::string& file);
void test_0_1_TSPLIB(const std::string& file);
void test_0_2_TSPLIB(const std::string& file);
void test_1_2_TSPLIB(const std::string& file);
void test_2_2_TSPLIB(const std::string& file);
void test_Neigh_TSPLIB(const std::string& file);

//Mass Comparison Part
void mass_test_0_1_TSPLIB(std::string fileName);
void mass_test_0_2_TSPLIB(std::string fileName);
void mass_test_1_2_TSPLIB(std::string fileName);
void mass_test_02_12_TSPLIB(std::string fileName);
void mass_test_Neigh_TSPLIB(std::string fileName);

#endif //ALME_TESTER_H
