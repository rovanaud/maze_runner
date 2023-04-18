#include "draw_walls.hpp"

using namespace cgp;

mesh draw_wall(vec2 p1, vec2 p2, float h, float e) {
	mesh wall;

	// normal vector
	vec3 n = normalize(vec3({ p2.y - p1.y, p1.x - p2.x, 0 }));
	vec3 p = { p1.x, p1.y, 0 };
	vec3 q = { p2.x, p2.y, 0 };
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

	std::cout <<  "Wall created" << std::endl;

	return wall;	
}

mesh create_maze(std::vector<std::vector<cgp::vec2>> points, float heigh, float e) {
	mesh maze;
	for (auto pair : points) {
		maze.push_back(draw_wall(pair[0], pair[1], heigh, e));
	}
	std::cout << "Maze created" << std::endl;
	return maze;
}