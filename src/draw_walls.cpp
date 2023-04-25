#include "draw_walls.hpp"

using namespace cgp;

mesh draw_wall(vec2 p1, vec2 p2, float h, float e) {
	mesh_drawable wall;
	// Points of the basis
	vec3 s1 = { p1.x - e, p1.y, 0 };
	vec3 s2 = { p2.x + e, p2.y, 0 };
	vec3 s3 = { p2.x + e, p2.y, 0 };
	vec3 s4 = { p1.x + e, p1.y, 0 };

	// Points of the top
	vec3 s5 = { p1.x - e, p1.y, h };
	vec3 s6 = { p2.x + e, p2.y, h };
	vec3 s7 = { p2.x + e, p2.y, h };
	vec3 s8 = { p1.x + e, p1.y, h };

	// Mesh construction
	wall.push_back(mesh_primitive_quad(s1, s2, s3, s4));
	wall.push_back(mesh_primitive_quad(s5, s6, s7, s8));
	wall.push_back(mesh_primitive_quad(s1, s2, s6, s5));
	wall.push_back(mesh_primitive_quad(s2, s3, s7, s6));
	wall.push_back(mesh_primitive_quad(s3, s4, s8, s7));
	wall.push_back(mesh_primitive_quad(s4, s1, s5, s8));

	return wall;	
}

mesh create_maze(std::vector<std::vector<cgp::vec2>> points, float heigh, float e) {
	mesh maze;
	for (auto pair : points) {
		maze.push_back(draw_wall(pair[0], pair[1], heigh, e));
	}
	return maze;
}