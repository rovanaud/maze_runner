#pragma once

#include "cgp/cgp.hpp"

cgp::mesh draw_wall(std::vector<int> p1, std::vector<int> p2, float h, float e);
cgp::mesh create_maze(std::vector<std::vector<std::vector<int>>> , float, float);