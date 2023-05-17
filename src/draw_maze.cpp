#include "draw_maze.hpp"

using namespace cgp;

mesh draw_wall(std::vector<int> p1, std::vector<int> p2, float h, float e) {
	mesh wall;

	// normal vector
	vec3 n = normalize(vec3({ p2[1] - p1[1], p1[0] - p2[0], 0}));
	vec3 p = { p1[0], p1[1], 0 };
	vec3 q = { p2[0], p2[1], 0};
	vec3 z = { 0, 0, 1 };
	// Points of the basis
	vec3 s1 = p + e * n;
	vec3 s2 = q + e * n;
	vec3 s3 = q - e * n;
	vec3 s4 = p - e * n;

	// Points of the top
	vec3 s5 = s1 + h * z;
	vec3 s6 = s2 + h * z;
	vec3 s7 = s3 + h * z;
	vec3 s8 = s4 + h * z;

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

mesh create_maze(std::vector<std::vector<std::vector<int>>> points, float heigh, float e) {
	mesh maze;
	float max_x = -std::round_toward_neg_infinity;
	float max_y = std::round_toward_neg_infinity;
	for (auto pair : points) {
		maze.push_back(draw_wall(pair[0], pair[1], heigh, e));
		max_x = std::max(max_x, (float)std::max(pair[0][0], pair[1][0]));
		max_y = std::min(max_y, (float)std::min(pair[0][1], pair[1][1]));
	}
	
	//std::cout << "Maze created" << std::endl;
	maze.apply_translation_to_position( vec3({ -max_x / 2, -max_y / 2, 0 }) );
	std::cout << 1 << std ::endl; 
	mesh ground = mesh_primitive_quadrangle(vec3(-max_x / 2, -max_y / 2, 0), vec3(max_x / 2, -max_y / 2, 0), vec3(max_x / 2, max_y / 2, 0), vec3(-max_x / 2, max_y / 2, 0));
	std::cout << 2 << std :: endl;
	ground.color.fill({ 0.9f, 0.5f, 0.01f });
	maze.push_back(ground);

	return maze;
}