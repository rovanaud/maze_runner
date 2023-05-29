
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include <iostream>

#include "util.hpp"
#include "const.hpp"

// mur sup: wall[0]
// mur inf: wall[1]
// mur gauche: wall[2]
// mur droite: wall[3]


using namespace std;

void initMaze() {
    mazeVect.resize(WIDTH, vector<Cell>(HEIGHT));
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            mazeVect[i][j].visited = false;
            for (int k = 0; k < 4; k++) {
                mazeVect[i][j].walls[k] = true;
            }
        }
    }
}

vector <int> generateMaze() {

    mazeVect[0][0].walls[2] = false;
    mazeVect[0][14].walls[3] = false;

    std::stack<std::pair<int, int>> stack;
    int totalCells = WIDTH * HEIGHT;
    int visitedCells = 1;
    std::pair<int, int> currentCell = std::make_pair(0, 0);
    mazeVect[currentCell.first][currentCell.second].visited = true;

    while (visitedCells < totalCells) {
        std::vector<std::pair<int, int>> neighbors;

        // Check neighboring cells
        if (currentCell.first > 0 && !mazeVect[currentCell.first - 1][currentCell.second].visited) {
            neighbors.push_back(std::make_pair(currentCell.first - 1, currentCell.second));
        }
        if (currentCell.first < WIDTH - 1 && !mazeVect[currentCell.first + 1][currentCell.second].visited) {
            neighbors.push_back(std::make_pair(currentCell.first + 1, currentCell.second));
        }
        if (currentCell.second > 0 && !mazeVect[currentCell.first][currentCell.second - 1].visited) {
            neighbors.push_back(std::make_pair(currentCell.first, currentCell.second - 1));
        }
        if (currentCell.second < HEIGHT - 1 && !mazeVect[currentCell.first][currentCell.second + 1].visited) {
            neighbors.push_back(std::make_pair(currentCell.first, currentCell.second + 1));
        }

        if (!neighbors.empty()) {
            // Choose a random neighbor
            int randomIndex = rand() % neighbors.size();
            std::pair<int, int> nextCell = neighbors[randomIndex];

            // Remove the wall between current cell and next cell
            if (nextCell.first == currentCell.first - 1) {
                //std::cout << "up " << currentCell.first << ',' << currentCell.second << endl;
                mazeVect[currentCell.first][currentCell.second].walls[0] = false;
                mazeVect[nextCell.first][nextCell.second].walls[1] = false;
            }
            else if (nextCell.first == currentCell.first + 1) {
                //std::cout << "down " << currentCell.first << ',' << currentCell.second << endl;
                mazeVect[currentCell.first][currentCell.second].walls[1] = false;
                mazeVect[nextCell.first][nextCell.second].walls[0] = false;
            }
            else if (nextCell.second == currentCell.second - 1) {
                //std::cout << "gauche " << currentCell.first << ',' << currentCell.second << endl;
                mazeVect[currentCell.first][currentCell.second].walls[2] = false;
                mazeVect[nextCell.first][nextCell.second].walls[3] = false;
            }
            else if (nextCell.second == currentCell.second + 1) {
                //std::cout << "droite " << currentCell.first << ',' << currentCell.second << endl;
                mazeVect[currentCell.first][currentCell.second].walls[3] = false;
                mazeVect[nextCell.first][nextCell.second].walls[2] = false;
            }

            // Update the current cell
            currentCell = nextCell;
            mazeVect[currentCell.first][currentCell.second].visited = true;
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

void printMaze() {
    cout << " ";
    for (int i = 0; i < WIDTH * 2 - 1; i++) {
        cout << "_";
    }
    cout << endl;
    for (int i = 0; i < HEIGHT; i++) {
        cout << "|";
        for (int j = 0; j < WIDTH; j++) {
            if (mazeVect[i][j].walls[1]) {
                cout << "_";
            }
            else {
                cout << " ";
            }
            if (mazeVect[i][j].walls[3]) {
                cout << "|";
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

bool possible_move(float x, float y) {
	/*

		double up = std::round(player.model.translation.y) + 0.5f * (HEIGHT % 2);
		double left = std::round(player.model.translation.x) - 0.5f * (WIDTH % 2);

		int i = WIDTH / 2  + std::floor(player.model.translation.x - 0.5 * (WIDTH % 2));
		int j = HEIGHT / 2 - std::ceil(player.model.translation.y - 0.5 * (HEIGHT % 2));

		float d_min = 0.08f;
		// up collision
		if ((std::abs(player.model.translation.y - up) < d_min) && mazeVect[i][j].walls[0])
			return true;
		// down collision
		if ((std::abs(player.model.translation.y - up - 1) < d_min) && mazeVect[i][j].walls[1])
			return true;
		// left collision
		if ((std::abs(player.model.translation.x - left) < d_min) && mazeVect[i][j].walls[2])
			return true;
		// right collision
		if ((std::abs(player.model.translation.x - left - 1) < d_min) && mazeVect[i][j].walls[3])
			return true;
		cout << "no collision" << endl;
		return false;

	*/

	double w_half(0.5f * (WIDTH % 2));
	double h_half((HEIGHT % 2) * .5f);


	double up = h_half + std::ceil(y - h_half); // smallest integer greater than or equal to y
	double left = w_half + std::floor(x - w_half); // largest integer less than or equal to x


	// C'est un peu compliqué ici parce que les indices de la matrices sont inversés...
	int i = std::round(HEIGHT / 2 - up + h_half); // round to the nearest integer
	int j = std::round(WIDTH / 2 + left + w_half); // round to the nearest integer


	float d_min = 0.05f; // epaisseur du mur / 2 plus un epsilon

	// up collision
	if ((up - y < d_min) && mazeVect[i][j].walls[0])
		return false;
	// down collision
	if ((y - up + 1 < d_min) && mazeVect[i][j].walls[1])
		return false;
	// left collision
	if ((x - left < d_min) && mazeVect[i][j].walls[2])
		return false;
	// right collision
	if ((left + 1 - x < d_min) && mazeVect[i][j].walls[3])
		return false;

	return true;

}

random::random() {
    randomEngine.seed(std::random_device()());
}

int random::randint(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(randomEngine);
}

double random::randreal(double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(randomEngine);
}

namespace cgp {
    void maze_into_connectedPoints() {
        float dx = WIDTH / 2.0f;
        float dy = HEIGHT / 2.0f;
        connectedPoints.push_back(vector<vec2> { vec2(-dx, dy), vec2(HEIGHT - dx, dy) });
        connectedPoints.push_back(vector<vec2> { vec2(-dx, dy - 1), vec2(-dx, -WIDTH + dy) });

        for (int i = 0; i < HEIGHT; i++) { //i = y (hauteur) et j = x (longueur)
            for (int j = 0; j < WIDTH; j++) {
                if (mazeVect[i][j].walls[1]) {
                    connectedPoints.push_back(vector<vec2> { vec2(j - dx, dy - (i + 1)), vec2(j - dx + 1, dy - (i + 1)) }); // y en premier et x en deuxième
                }
                if (mazeVect[i][j].walls[3]) {
                    connectedPoints.push_back(vector<vec2> { vec2(j - dx + 1, dy -  i), vec2(j - dx + 1, dy - (i + 1))});
                }
            }
    }
    //return connectedPoints;
}
    
    mesh draw_wall(vec2 p1, vec2 p2, float h, float e) {
        mesh wall;

        // normal vector
        vec3 n = 0.5f * e * normalize(vec3({ p2.y - p1.y, p1.x - p2.x, 0 }));
        vec3 p = { p1.x, p1.y, 0 };
        vec3 q = { p2.x, p2.y, 0 };
        vec3 z = { 0, 0, h };
        // Points of the basis
        vec3 s1 = p + n;
        vec3 s2 = q + n;
        vec3 s3 = q - n;
        vec3 s4 = p - n;

        // Points of the top
        vec3 s5 = s1 + z;
        vec3 s6 = s2 + z;
        vec3 s7 = s3 + z;
        vec3 s8 = s4 + z;

        // Mesh construction
        wall.push_back(mesh_primitive_quadrangle(s1, s2, s3, s4));
        wall.push_back(mesh_primitive_quadrangle(s5, s6, s7, s8));
        wall.push_back(mesh_primitive_quadrangle(s1, s2, s6, s5));
        wall.push_back(mesh_primitive_quadrangle(s2, s3, s7, s6));
        wall.push_back(mesh_primitive_quadrangle(s3, s4, s8, s7));
        wall.push_back(mesh_primitive_quadrangle(s4, s1, s5, s8));

        //std::cout <<  "Wall created" << std::endl;

        return wall;
    }

    mesh create_maze(float heigh, float e) {
        mesh maze;
        //std::vector<std::vector<std::vector<int>>> points = ...
        maze_into_connectedPoints();

        float max_x = std::round_toward_infinity;
        float min_y = std::round_toward_neg_infinity;
        for (auto pair : connectedPoints) {
            maze.push_back(draw_wall(pair[0], pair[1], heigh, e));
            max_x = std::max(max_x, std::max(pair[0].x, pair[1].x));
            min_y = std::min(min_y, std::min(pair[0].y, pair[1].y));
        }

        //std::cout << "Maze created" << std::endl;
        //maze.apply_translation_to_position(vec3({ -max_x / 2, -min_y / 2, 0 }));
        std::cout << "max_x" << max_x << "min_y" << min_y << std::endl;
        mesh ground = mesh_primitive_quadrangle(
            vec3(-max_x , -min_y, 0),
            vec3(-max_x ,  min_y, 0),
            vec3( max_x ,  min_y, 0),
            vec3( max_x , -min_y, 0)
        );
        ground.color.fill({ 0.9f, 0.5f, 0.01f });
        maze.push_back(ground);
        return maze;
    }

    vector<vec2> getConnectedPoint(int i) {
        return connectedPoints[i];
    }

    hierarchy_mesh_drawable draw_spider(cgp::hierarchy_mesh_drawable& spider)
    {
        cout << '5' << std::endl;

        mesh_drawable ellipsoid;
        mesh_drawable sphere;
        mesh_drawable sphere_1;

        mesh_drawable cylinder_leg;
        mesh_drawable cylinder_foot;

        // Create the geometry of the meshes
        //   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)


        ellipsoid.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.08 ,0.05,0.05 }));
        sphere.initialize_data_on_gpu(mesh_primitive_sphere(0.03f));
        cylinder_leg.initialize_data_on_gpu(mesh_primitive_cylinder(0.005f, { 0,0,0 }, { 0,0,0.05 }));
        cylinder_foot.initialize_data_on_gpu(mesh_primitive_cylinder(0.005f, { 0,0,0 }, { 0,0,0.1 }));


        sphere_1.initialize_data_on_gpu(mesh_primitive_sphere(0.005f));


        // Set the color of some elements

        vec3 orange = { 1,0.5,0 };
        vec3 black = { 0,0,0 };
        sphere_1.material.color = orange;

        cylinder_leg.material.color = black;
        cylinder_foot.material.color = black;

        ellipsoid.material.color = black;
        sphere.material.color = black;

        // Add the elements in the hierarchy
        //   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
        //   Notes: 
        //     - An element must necessarily be added after its parent
        //     - The first element (without explicit name of its parent) is assumed to be the root.

        cout << '6' << std::endl;

        spider.add(ellipsoid, "body");
        spider.add(sphere, "head", "body", { 0.1,0,0.02 });
        spider.add(sphere_1, "eye1", "head", { 0.02 ,-0.015,0.01 });
        spider.add(sphere_1, "eye2", "head", { 0.02 ,0.015,0.01 });

        spider.add(cylinder_leg, "leg_1", "body", { 0 ,0.03,0 }, rotation_transform::from_axis_angle({ 1,0,0 }, -1));
        spider.add(cylinder_foot, "foot_1", "leg_1", { 0,-0.004,0.048 });

        spider.add(cylinder_leg, "leg_2", "body", { 0.04 ,0.03,0 }, rotation_transform::from_axis_angle({ 1,0,0 }, -1));
        spider.add(cylinder_foot, "foot_2", "leg_2", { 0,-0.004,0.048 });

        spider.add(cylinder_leg, "leg_3", "body", { -0.04 ,0.03,0 }, rotation_transform::from_axis_angle({ 1,0,0 }, -1));
        spider.add(cylinder_foot, "foot_3", "leg_3", { 0,-0.004,0.048 });

        spider.add(cylinder_leg, "leg_4", "body", { 0 ,-0.03, 0 }, rotation_transform::from_axis_angle({ 1,0,0 }, 1));
        spider.add(cylinder_foot, "foot_4", "leg_4", { 0,0.004,0.048 });

        spider.add(cylinder_leg, "leg_5", "body", { -0.04, -0.03 ,0 }, rotation_transform::from_axis_angle({ 1,0,0 }, 1));
        spider.add(cylinder_foot, "foot_5", "leg_5", { 0,0.004,0.048 });

        spider.add(cylinder_leg, "leg_6", "body", { 0.04 ,-0.03, 0 }, rotation_transform::from_axis_angle({ 1,0,0 }, 1));
        spider.add(cylinder_foot, "foot_6", "leg_6", { 0,0.004,0.048 });

        cout << '7' << std::endl;

        return spider; 

    }
}



int getSizeConnectedPoints() {
	return connectedPoints.size();
}