//
// Created by Gabriel on 11.03.2022.
//

#ifndef ALME_PARSER_H
#define ALME_PARSER_H

#include <utility>
#include <vector>
#include <cstdio>
#include <string>

std::vector<std::pair<double, double>> parse_coords(const std::string& file);
int** coords_to_matrix(std::vector<std::pair<double, double>> coords);
std::pair<int**, int> parse_matrix(const std::string& file);

#endif //ALME_PARSER_H
