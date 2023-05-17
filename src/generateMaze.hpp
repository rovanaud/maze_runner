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

void initMaze(Cell maze[][WIDTH]);
bool allvisited(Cell maze[][WIDTH]);
bool inBounds(int row, int col);
void removeWall(Cell maze[][WIDTH], int row, int col, Direction dir);
vector<int> generateMaze(Cell maze[][WIDTH]);
void printMaze(Cell maze[][WIDTH]);
vector<vector<vector<int>>> maze_into_connected_points(Cell[][WIDTH], vector<vector<int>>& walls);