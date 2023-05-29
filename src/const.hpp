#ifndef CONST_HPP
#define CONST_HPP


enum Direction { UP, DOWN, LEFT, RIGHT };

struct Cell {
    bool visited;
    bool walls[4];
};


constexpr int WIDTH = 15;
constexpr int HEIGHT = 15;

std::vector<std::vector<Cell> > mazeVect;

vector<vector<cgp::vec2>> connectedPoints;

#endif // !CONST_HPP