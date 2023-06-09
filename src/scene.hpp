#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "camera.hpp"
#include "util.hpp"
#include "key_positions_structure.hpp"

// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::numarray;
using cgp::timer_basic;


// Variables associated to the GUI
struct gui_parameters {
	bool display_frame = false;
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

	//camera_controller_first_person camera_controller_first_person;
	maze_camera_controller camera_controller_first_person;
	//camera_controller_first_person_euler

	mesh_drawable global_frame;          // The standard global frame
	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	//cgp::mesh_drawable maze;
	cgp::mesh_drawable mazeMesh;

	mesh_drawable beast;
	vector<vec3> positions;

	cgp::hierarchy_mesh_drawable hierarchy;

	cgp::skybox_drawable skybox;

	// ****************************** //
	// Functions
	// ****************************** //

	bool first_person = true;


	// A helper structure used to store and display the key positions/time

	keyframe_structure keyframe;

	cgp::timer_interval timer1;

	timer_basic timer;

	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();   // The display of the GUI, also called within the animation loop


	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();
	vector<vec3> generatePath3D(double cubeSize, double minDistance);

};





