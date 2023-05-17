#include "generateMaze.hpp"

//
// mur inf: wall[1]
// mur sup: wall[0]
// mur droite: wall[3]
// mur gauche: wall[2]
//

using namespace std;


void initMaze(Cell maze[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            maze[i][j].visited = false;
            for (int k = 0; k < 4; k++) {
                maze[i][j].walls[k] = true;
            }
        }
    }
}

bool allvisited(Cell maze[][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (!maze[i][j].visited) {
                return false;
            }
        }
    }
    return true;
}

bool inBounds(int row, int col) {
    return row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH;
}

void removeWall(Cell maze[][WIDTH], int row, int col, Direction dir) {
    switch (dir) {
    case UP:
        maze[row][col].walls[0] = false;
        maze[row - 1][col].walls[1] = false;
        break;
    case DOWN:
        maze[row][col].walls[1] = false;
        maze[row + 1][col].walls[0] = false;
        break;
    case LEFT:
        maze[row][col].walls[2] = false;
        maze[row][col - 1].walls[3] = false;
        break;
    case RIGHT:
        maze[row][col].walls[3] = false;
        maze[row][col + 1].walls[2] = false;
        break;
    }
}

void generateMaze(Cell maze[][WIDTH]) {
    stack<pair<int, int>> cells;
    int startRow = rand() % HEIGHT;
    int startCol = rand() % WIDTH;
    cells.push(make_pair(startRow, startCol));
    maze[startRow][startCol].visited = true;
    while (!cells.empty() && !allvisited(maze)) {
        pair<int, int> current = cells.top();
        cells.pop();
        int row = current.first;
        int col = current.second;
        int neighbors[4][2] = { {row - 1, col}, {row + 1, col}, {row, col - 1}, {row, col + 1} };
        random_shuffle(&neighbors[0], &neighbors[4]);
        for (int i = 0; i < 4; i++) {
            int nrow = neighbors[i][0];
            int ncol = neighbors[i][1];
            if (inBounds(nrow, ncol) && !maze[nrow][ncol].visited) {
                cells.push(make_pair(nrow, ncol));
                removeWall(maze, row, col, static_cast<Direction>(i));
                maze[nrow][ncol].visited = true;
            }
        }
    }
}

void printMaze(Cell maze[][WIDTH]) {
    cout << " ";
    for (int i = 0; i < WIDTH * 2 - 1; i++) {
        cout << "_";
    }
    cout << endl;
    for (int i = 0; i < HEIGHT; i++) {
        cout << "|";
        for (int j = 0; j < WIDTH; j++) {
            if (maze[i][j].walls[1]) {
                cout << "_";
            }
            else {
                cout << " ";
            }
            if (maze[i][j].walls[3]) {
                cout << "|";
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

vector<vector<vector<int>>> maze_into_connected_points(Cell maze[][WIDTH]) {
    /*Cell maze[HEIGHT][WIDTH];
	initMaze(maze);
	generateMaze(maze);*/
	vector<vector<vector<int>>> connected_points;
    connected_points.push_back(vector<vector<int>> { {0, 0}, { 0, HEIGHT - 1 } });
    connected_points.push_back(vector<vector<int>> { {0, 0}, { WIDTH - 1, 0 } });
    connected_points.push_back(vector<vector<int>> { { 0, HEIGHT - 1}, { HEIGHT - 1, WIDTH - 1 } });
    connected_points.push_back(vector<vector<int>> { { WIDTH - 1, HEIGHT - 1 }, { WIDTH - 1, 0 }});
    
    for (int i = 0; i < HEIGHT - 1; i++) {
        int p1(0), p2(0);
        int j = 0;
        while (j < WIDTH) {
            if (maze[i][j].walls[0]) {
                p2 = j + 1;
            }
            else {
                if (p1 != p2) {
                    connected_points.push_back(vector<vector<int>> { { i, p1}, { i , p2 } });
                }
                p1 = p2 = j + 1;
            }  
            ++j;
        }
	}
    for (int i = 0; i < WIDTH - 1; i++) {
        int p1(0), p2(0);
        int j = 0;
        while (j < HEIGHT) {
            if (maze[i][j].walls[2]) {
                p2 = j + 1;
            }
            else {
                if (p1 != p2) {
                    connected_points.push_back(vector<vector<int>> { { p1, i }, { p2, i  } });
                }
                p1 = p2 = j + 1;
            }     
            ++j;
        }
	}
	return connected_points;
}