/*******************/
// MAZE 
//******************// 

#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const int WIDTH = 15;
const int HEIGHT = 15;

enum Direction { UP, DOWN, LEFT, RIGHT };

//struct Cell;
//struct Player;

struct Cell {
    bool visited;
    bool walls[4];
};

struct Player {
    Cell pos;
};

void initMaze(Cell maze[][WIDTH]);
bool allvisited(Cell maze[][WIDTH]);
bool inBounds(int row, int col);
void removeWall(Cell maze[][WIDTH], int row, int col, Direction dir);
void generateMaze(Cell maze[][WIDTH]);
void printMaze(Cell maze[][WIDTH]);
vector<vector<vector<int>>> maze_into_connected_points(Cell[][WIDTH]);
