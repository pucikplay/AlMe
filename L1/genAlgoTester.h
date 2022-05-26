//
// Created by Franek on 26.05.2022.
//

#ifndef GENALGOTESTER_H
#define GENALGOTESTER_H

#include "parser.h"
#include <vector>
#include <fstream>
#include <cstring>
#include <iostream>
#include "genAlgo.h"

//Mode tests on 1 instance
void geneticModeTestSimetric(std::string fileName);
void geneticModeTestAsimetric(std::string fileName);

//Parameter testings on 1 instance
void geneticParseekerSimetric(std::string fileName);
void geneticParseekerAsimetric(std::string fileName);

//Age testing

//Local Enhancer as mutation testing
void geneticLocEnhaTesterSimetric(std::string fileName);
void geneticLocEnhaTesterAsimetric(std::string fileName);

//Tabu data

//Island test

#endif //GENALGOTESTER_H
