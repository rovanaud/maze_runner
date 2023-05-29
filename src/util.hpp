#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include "cgp/cgp.hpp"

#ifndef UTIL_HPP // header guards
#define UTIL_HPP

using namespace std;

void initMaze(); //pas moyen de faire un pointeur ? 
vector<int> generateMaze();
void printMaze();

bool possible_move(float x, float y);

class random {
public:
    random();

    int randint(int min, int max);

    double randreal(double min, double max);

private:
    std::default_random_engine randomEngine;
};

namespace cgp{
    void maze_into_connectedPoints();
    mesh draw_wall(vec2 p1, vec2 p2, float h, float e);
    mesh create_maze(float, float);
    vector<vec2> getConnectedPoint(int i);
}



int getSizeConnectedPoints();
#endif