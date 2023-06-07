#include "scene.hpp"

//using namespace cgp;
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::vec2;
using std::cout;
using std::endl;
using std::vector;


void scene_structure::initialize() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());
	
	float h = .50f;
	float e = .01f;
	float r = 0.08f;

	initMaze();
	vector<int> start = generateMaze(); 
	printMaze();
 
	
	mesh maze_mesh = create_maze(h, e); //draw_wall(p1, p2, h, e); 

	mazeMesh.initialize_data_on_gpu(maze_mesh);
	mazeMesh.material.color = vec3({ 0.2f, 0.9f, 0.7f });

	beast.initialize_data_on_gpu(mesh_primitive_sphere(0.01f));
	positions = vector<vec3>(getSizeConnectedPoints());
	for (int i = 0; i < getSizeConnectedPoints(); i++) {
		//beasts[i].initialize_data_on_gpu(mesh_primitive_sphere(0.01f));
		//beasts[i].material.color = vec3({0.9f, 0.2f, 0.2f});
		positions[i]= .5f * (vec3(getConnectedPoint(i)[0], h)  + vec3(getConnectedPoint(i)[1], h));
		//cout << "beasts " << i + 1 << " : " << beasts[i].model.translation << endl;
	}

	// sky box
	image_structure image_skybox_template = image_load_file("assets/skybox_02.jpg");
	// Other possibilities:
	//   image_structure image_skybox_template = image_load_file("assets/skybox_02.jpg");
	//   image_structure image_skybox_template = image_load_file("assets/skybox_debug.png");


	// Split the image into a grid of 4 x 3 sub-images
	std::vector<image_structure> image_grid = image_split_grid(image_skybox_template, 4, 3);


	skybox.initialize_data_on_gpu();
	skybox.texture.initialize_cubemap_on_gpu(image_grid[1], image_grid[7], image_grid[5], image_grid[3], image_grid[10], image_grid[4]);
	// Look at skybox_debug.png to see the correspondance of the image index
	
	// Finally, setup the camera
	first_person = true;
	if (first_person) {
		camera_controller_first_person.initialize(inputs, window);
		// mazeMesh.material.alpha = 0.2f;
		
		// place player at the begining of the maze
		vec3 zaxis(0, 0, 1);
		vec3 st(start[0], start[1], r/2); 
		vec3 center = st + vec3(r * sqrt(2), r * sqrt(2), 0); // Position of camera
		cout << "le début du lab : (" <<  start[0] << ", " << start[1] <<")"<< endl;
		
		// Look at the player, from behind
		camera_controller_first_person.camera_model.look_at(center, st, zaxis);
		
		// set maze
		// camera_controller_first_person.set_maze(mazeVect, maze);
		
	}
	else {
		camera_control.initialize(inputs, window);
		camera_control.look_at(vec3(0, 0, 20), vec3(0, 0, 0), vec3(0, 0, 1));
	}
}

void scene_structure::display_frame()
{
	//draw(sphere_light, environment);

	// Set the light to the current position of the camera
	if (first_person) 
		environment.light = camera_controller_first_person.camera_model.position(); //comment faire pour positionner la caméra à l'endroit souhaité ? 
	else
		environment.light = camera_control.camera_model.position();
	
	//if (gui.display_frame)
	//	draw(global_frame, environment);
	// update coordinate vefore drawing
	// hierarchy.update_local_to_global_coordinates();

	//  Must be called before drawing the other shapes and without writing in the Depth Buffer
	glDepthMask(GL_FALSE); // disable depth-buffer writing
		draw(skybox, environment);
	glDepthMask(GL_TRUE);  // re-activate depth-buffer write

	timer.update();
	
	// draw(hierarchy, environment);
	draw(mazeMesh, environment);
	
	for (int i = 0; i < getSizeConnectedPoints(); i++) {
		beast.model.translation = positions[i];
		draw(beast, environment);
	}

	if (gui.display_wireframe){
		draw_wireframe(mazeMesh, environment);
		//draw_wireframe(beast, environment);
	}

	timer.update();

	
	for (int i = 0; i < getSizeConnectedPoints(); i++) {
		vector<vec2> points = getConnectedPoint(i);
		vec3 p1 = vec3(points[0], 0.25f);
		vec3 p2 = vec3(points[1], 0.25f);
		vec3 m = 0.5f * (p2 + p1);
		vec3 v = 0.5f * (p2 - p1);
		positions[i] = m + cos(timer.t * 0.02) * v + vec3(0, 0, 0.25f * sin(timer.t) * sin(timer.t));
		//beast.model.translation = positions[i];
		//draw(beast, environment);
	}
	
}

void scene_structure::display_gui()
{
	/*ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);*/

}

void scene_structure::mouse_move_event()
{
	//if (!inputs.keyboard.shift)
		//camera_control.action_mouse_move(environment.camera_view);
	if (first_person)
		camera_controller_first_person.action_mouse_move(environment.camera_view);
	else
		camera_control.action_mouse_move(environment.camera_view);
}

void scene_structure::mouse_click_event()
{
	if (!first_person) 
		camera_control.action_mouse_click(environment.camera_view);
	else
		camera_controller_first_person.action_mouse_click(environment.camera_view);
}

void scene_structure::keyboard_event()
{
	if (first_person) camera_controller_first_person.action_keyboard(environment.camera_view);
	// else camera_control.action_keyboard(environment.camera_view);

	float r = 0.2f;
	// cout << player.model.matrix() << endl; 
	// player.model.rotation.data = camera_controller_first_person.camera_model.orientation_camera.data;
	// player.model.translation = camera_controller_first_person.camera_model.position() - vec3(r * cos(Pi / 4), r * sin(Pi / 4), r / 2);
	
	float const magnitude = inputs.time_interval;
	float const angle_magnitude = inputs.time_interval;
	// //   front/back
	// if (inputs.keyboard.is_pressed(GLFW_KEY_W)){
	// 	player.model.translation += vec3( 0, magnitude, 0 );
	// 	// cout << "up : " << magnitude << endl;
	// }
	// if (inputs.keyboard.is_pressed(GLFW_KEY_S)){
	// 	player.model.translation += vec3( 0, -magnitude, 0 );
	// 	// cout << "down : " << magnitude << endl;
	// }
	// //   twist
	if (inputs.keyboard.is_pressed(GLFW_KEY_A)){
		// player.model.rotation = player.model.rotation.from_axis_angle(vec3(0, 0, 1), -angle_magnitude);
		// player.model.rotation *= rotation_transform::from_axis_angle({ 0, 0, 1 }, angle_magnitude);
		// cout << "left player : " << 180 * angle_magnitude / Pi << endl;
	}
	if (inputs.keyboard.is_pressed(GLFW_KEY_D)){
		// player.model.rotation = player.model.rotation.from_axis_angle(vec3(0, 0, 1), angle_magnitude);
		// player.model.rotation *= rotation_transform::from_axis_angle({ 0, 0, 1 }, -angle_magnitude);
		// cout << "right player : " << 180 * magnitude / Pi << endl;
	}

	// if (inputs.keyboard.left) {
	// 	player.model.translation += vec3( -magnitude, 0, 0 );
	// 	// cout << "left : " << magnitude << endl;
	// }
	// if (inputs.keyboard.right) {
	// 	player.model.translation += vec3( magnitude, 0, 0 );
	// 	// cout << "right : " << magnitude << endl;
	// }
	// if (inputs.keyboard.up) {
	// 	player.model.trif (collision_detection()) {
		//	camera_controller_first_person.no_collision = false;// cout << "collision" << endl;
		//}
		//if (!collision_detection()) {
		//	camera_controller_first_person.no_collision = true;// cout << "collision" << endl;
		//}anslation += vec3( 0, magnitude, 0 );
	// 	// cout << "up : " << magnitude << endl;
	// }
	// if (inputs.keyboard.down) {
	// 	player.model.translation += vec3( 0, -magnitude, 0 );
	// 	// cout << "down : " << magnitude << endl;
	// }	
}

void scene_structure::idle_frame()
{
	// cout << "come back to here" << endl;
	if (first_person){
		camera_controller_first_person.idle_frame(environment.camera_view);
		//
	}
	else 
	 	camera_control.idle_frame(environment.camera_view);
		
}