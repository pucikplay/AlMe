//
// Created by Gabriel on 26.03.2022.
//

#ifndef ALME_TESTER_H
#define ALME_TESTER_H

#include "parser.h"
#include "roadSolver.h"
#include "k_random.h"
#include <vector>
#include <ctime>
#include <fstream>
#include <cstring>

void test_0_1_TSPLIB(const std::string& file);
void test_0_2_TSPLIB(const std::string& file);
void test_1_2_TSPLIB(const std::string& file);

#endif //ALME_TESTER_H
