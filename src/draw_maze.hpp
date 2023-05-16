#pragma once

#include "cgp/cgp.hpp"

cgp::mesh draw_wall(cgp::vec2 p1, cgp::vec2 p2, float h, float e);
cgp::mesh create_maze(std::vector<std::vector<cgp::vec2>>, float, float);