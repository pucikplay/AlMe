//
// Created by Franek on 05.05.2022.
//

#ifndef ALME_TABU_TESTER_H
#define ALME_TABU_TESTER_H

#include "parser.h"
#include "2_opt.h"
#include "3_opt.h"
#include "roadSolver.h"
#include "k_random.h"
#include "tabu_search.h"
#include <vector>
#include <ctime>
#include <fstream>
#include <cstring>
#include <iostream>
#include <chrono>
#include "tabu_multithreaded_wrapper.h"

//Single Algorithm Part
void tabu_test_0_K_Rand(std::string fileName);

//Comparison Part
void tabu_test_Neigh_TSPLIB(const std::string& file);

//Parameters tuning
void parameter_tuner(const std::string& file);

//Mass Comparison Part
void test_variants_random(const std::string& file, int init_rode, int tabuSize, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode);

void test_variants_deterministic(const std::string& file, int init_rode, int tabuSize, size_t enhancementLimit, int mode, int kikMode, int kikSize);

void test_variants_parallel(const std::string& file, int init_rode, int tabuSize, size_t enhancementLimit, std::pair<size_t, size_t> kickRange, int mode);

void batch_test(const std::string& file1, const std::string& file2);

void mass_test_ltm(std::string fileName);

#endif //ALME_TABU_TESTER_H
