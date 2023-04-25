#pragma once

#include "cgp/cgp.hpp"

cgp::mesh draw_wall(cgp::vec2, cgp::vec2, float, float);
cgp::mesh create_maze(std::vector<std::vector<cgp::vec2>>, float, float);