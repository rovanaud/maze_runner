#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"

//*******************//
// MAZE 
//******************// 


const int WIDTH = 20;
const int HEIGHT = 20;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Cell;
struct Player; 

int startRow; 
int startCol; 

void initMaze(Cell maze[][WIDTH]);
bool allvisited(Cell maze[][WIDTH]);
bool inBounds(int row, int col);
void removeWall(Cell maze[][WIDTH], int row, int col, Direction dir);
void generateMaze(Cell maze[][WIDTH]);
void printMaze(Cell maze[][WIDTH]);



// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::numarray;
using cgp::timer_basic;

// Variables associated to the GUI
struct gui_parameters {
	bool display_frame = true;
	bool display_wireframe = false;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	camera_controller_orbit_euler camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;

	mesh_drawable global_frame;          // The standard global frame
	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable terrain;
	//cgp::mesh_drawable cylinder;

	//cgp::mesh_drawable tree;
	cgp::mesh_drawable tree;

	// cgp::mesh_drawable mushroom;
	cgp::mesh_drawable mushroom;

	// tree and mushrooms positions
	std::vector<cgp::vec3> tree_positions;

	//cgp::mesh_drawable maze;
	cgp::mesh_drawable maze;

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();   // The display of the GUI, also called within the animation loop


	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();

};





