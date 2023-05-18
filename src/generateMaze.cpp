#include "generateMaze.hpp"

//
// mur inf: wall[1]
// mur sup: wall[0]
// mur droite: wall[3]
// mur gauche: wall[2]
//

using namespace std;


void initMaze(vector<vector<Cell>>& maze) {
    maze.resize(WIDTH, vector<Cell>(HEIGHT));
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            maze[i][j].visited = false;
            for (int k = 0; k < 4; k++) {
                maze[i][j].walls[k] = true;
            }
        }
    }
}

vector <int> generateMaze(vector<vector<Cell>>& maze) {

    maze[0][0].walls[2] = false; 
    maze[0][14].walls[3] = false;

    std::stack<std::pair<int, int>> stack;
    int totalCells = WIDTH * HEIGHT;
    int visitedCells = 1;
    std::pair<int, int> currentCell = std::make_pair(0, 0);
    maze[currentCell.first][currentCell.second].visited = true;

    while (visitedCells < totalCells) {
        std::vector<std::pair<int, int>> neighbors;

        // Check neighboring cells
        if (currentCell.first > 0 && !maze[currentCell.first - 1][currentCell.second].visited) {
            neighbors.push_back(std::make_pair(currentCell.first - 1, currentCell.second));
        }
        if (currentCell.first < WIDTH - 1 && !maze[currentCell.first + 1][currentCell.second].visited) {
            neighbors.push_back(std::make_pair(currentCell.first + 1, currentCell.second));
        }
        if (currentCell.second > 0 && !maze[currentCell.first][currentCell.second - 1].visited) {
            neighbors.push_back(std::make_pair(currentCell.first, currentCell.second - 1));
        }
        if (currentCell.second < HEIGHT - 1 && !maze[currentCell.first][currentCell.second + 1].visited) {
            neighbors.push_back(std::make_pair(currentCell.first, currentCell.second + 1));
        }

        if (!neighbors.empty()) {
            // Choose a random neighbor
            int randomIndex = rand() % neighbors.size();
            std::pair<int, int> nextCell = neighbors[randomIndex];

            // Remove the wall between current cell and next cell
            if (nextCell.first == currentCell.first - 1 ) {
                std::cout << "up " << currentCell.first << ',' << currentCell.second << endl;
                maze[currentCell.first][currentCell.second].walls[0] = false;
                maze[nextCell.first][nextCell.second].walls[1] = false;
            }
            else if (nextCell.first == currentCell.first + 1) {
                std::cout << "down " << currentCell.first << ',' << currentCell.second << endl;
                maze[currentCell.first][currentCell.second].walls[1] = false;
                maze[nextCell.first][nextCell.second].walls[0] = false;
            }
            else if (nextCell.second == currentCell.second - 1) {
                std::cout << "gauche " << currentCell.first << ',' << currentCell.second << endl;
                maze[currentCell.first][currentCell.second].walls[2] = false;
                maze[nextCell.first][nextCell.second].walls[3] = false;
            }
            else if (nextCell.second == currentCell.second + 1) {
                std::cout << "droite " << currentCell.first << ',' << currentCell.second << endl;
                maze[currentCell.first][currentCell.second].walls[3] = false;
                maze[nextCell.first][nextCell.second].walls[2] = false;
            }

            // Update the current cell
            currentCell = nextCell;
            maze[currentCell.first][currentCell.second].visited = true;
            stack.push(currentCell);
            visitedCells++;
        }
        else if (!stack.empty()) {
            // Backtrack if there are no unvisited neighboring cells
            currentCell = stack.top();
            stack.pop();
        }
    }

    vector<int> start = { 0, 0 };
    return start;
}

void printMaze(vector<vector<Cell>>& maze) {
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


vector<vector<vector<int>>> maze_into_connected_points(vector<vector<Cell>>& maze) {
    /*Cell maze[HEIGHT][WIDTH];
    initMaze(maze);
    generateMaze(maze);*/
    vector<vector<vector<int>>> connected_points;

    connected_points.push_back(vector<vector<int>> { {0, 0}, { 15, 0 }});
    connected_points.push_back(vector<vector<int>> { {0, -1}, { 0, -15 }});

    for (int i = 0; i<HEIGHT; i++) { //i = y (hauteur) et j = x (longueur)
        for (int j = 0; j<WIDTH; j++)
        {
            if (maze[i][j].walls[1])
            {
                connected_points.push_back(vector<vector<int>> { {j, -(i+1)}, { j+ 1, -(i+1)}}); // y en premier et x en deuxième
            }
            if (maze[i][j].walls[3])
            {
                connected_points.push_back(vector<vector<int>> { {j+1, -i }, { j + 1, -(i+1)}});
            }
        }
    }
    return connected_points;
}

