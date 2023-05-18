#pragma once

///*******************/
// MAZE 
//******************// 

#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

constexpr int WIDTH = 15;
constexpr int HEIGHT = 15;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Cell {
    bool visited;
    bool walls[4];
};

void initMaze(vector<vector<Cell>>& maze); //pas moyen de faire un pointeur ? 
vector<int> generateMaze(vector<vector<Cell>>& maze);
void printMaze(std::vector<vector<Cell>>& maze);
vector<vector<vector<int>>> maze_into_connected_points(std::vector<vector<Cell>>& maze);