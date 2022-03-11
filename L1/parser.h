//
// Created by Gabriel on 11.03.2022.
//

#ifndef ALME_PARSER_H
#define ALME_PARSER_H

#include <utility>
#include <vector>
#include <cstdio>
#include <string.h>

std::vector<std::pair<double, double>> parse_coords(const std::string& file);
int** coords_to_matrix(std::vector<std::pair<double, double>> coords);

#endif //ALME_PARSER_H
