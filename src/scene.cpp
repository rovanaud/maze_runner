#include "scene.hpp"
#include "generateMaze.hpp"
#include "terrain.hpp"
#include "tree.hpp"
#include "draw_maze.hpp"

using namespace cgp;
using namespace std;


void scene_structure::initialize() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());
	
	float h = 1.0f;
	float e = .3f;
	float r = 0.08f;
	//Maze Setup
	initMaze(maze_test);
	vector<int> start = generateMaze(maze_test); 
	printMaze(maze_test);
 
	mesh maze_mesh = create_maze(maze_into_connected_points(maze_test), .4f, 0.01f); //draw_wall(p1, p2, h, e); 

	maze.initialize_data_on_gpu(maze_mesh);
	maze.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_grass.jpg",
		GL_MIRRORED_REPEAT,
		GL_MIRRORED_REPEAT);

	maze.material.color = vec3({ 0.2f, 0.9f, 0.7f });
	
	// Player Setup
	vec3 player_G  = vec3(0, 0, 0);
	mesh player_mesh = mesh_primitive_sphere(r, player_G);
	player_mesh.color.fill({ 0.9f, 0.8f, 0.1f });
	player.initialize_data_on_gpu(player_mesh);
	// player.material.color = vec3({ 0.9f, 0.f, 0.f });


	// Finally, setup the camera
	first_person = true;
	if (first_person) {
		camera_controller_first_person.initialize(inputs, window);
		// maze.material.alpha = 0.2f;
		
		// place player at the begining of the maze
		vec3 zaxis(0, 0, 1);
		//vec3 st(start[0], start[1], r/2); // Position of the player
		vec3 st(2, 2.5, r/2); // Position of the player
		player.model.translation = st;
		vec3 center = st + vec3(r * sqrt(2), r * sqrt(2), 0); // Position of camera
		double distance = 0.4f; // Distance between the player and the camera
		cout << "le début du lab : (" <<  start[0] << ", " << start[1] <<")"<< endl;
		
		// Look at the player, from behind
		camera_controller_first_person.camera_model.look_at(st, center, zaxis);

		camera_controller_first_person.camera_model.position_camera = st - distance * camera_controller_first_person.camera_model.front();
		player.model.rotation = rotation_transform(camera_controller_first_person.camera_model.orientation_camera);
		camera_controller_first_person.set_player(player);

		
	}
	else {
		player.model.translation = vec3(2, 2.5, r/2);
		cout << "collision :  at " << player.model.translation << " : " << collision_detection() << '\n';
		player.model.translation = vec3(0.5, 2.5, r/2);
		cout << "collision :  " << player.model.translation << " : " << collision_detection() << '\n';
		player.model.translation = vec3(2.5, 6.5, r/2);
		cout << "collision :  " << player.model.translation << " : " << collision_detection() << '\n';
		player.model.translation = vec3(-1, -2.5, r/2);
		cout << "collision :  " << player.model.translation << " : " << collision_detection() << '\n';
		camera_control.initialize(inputs, window);
		camera_control.look_at(vec3(0, 0, 20), vec3(0, 0, 0), vec3(0, 0, 1));
	}

	// Sphere used to display the position of a light
	sphere_light.initialize_data_on_gpu(mesh_primitive_sphere(0.2f));
	
}

void scene_structure::display_frame()
{

	// Set additional uniform parameters to the shader
	environment.uniform_generic.uniform_float["ambiant"] = gui.ambiant;
	environment.uniform_generic.uniform_vec3["light_color"] = gui.light_color;
	environment.uniform_generic.uniform_vec3["light_position"] = gui.light_position;
	environment.uniform_generic.uniform_float["diffuse"] = gui.diffuse;
	environment.uniform_generic.uniform_float["specular"] = gui.specular;
	environment.uniform_generic.uniform_float["specular_exp"] = gui.specular_exp;
	environment.uniform_generic.uniform_vec3["couleur_brume"] = environment.background_color;

	sphere_light.model.translation = gui.light_position;
	sphere_light.material.color = gui.light_color * 0.8f;
	sphere_light.material.phong.ambient = 1;
	sphere_light.material.phong.diffuse = 0;
	sphere_light.material.phong.specular = 0;
	draw(sphere_light, environment);


	// Set the light to the current position of the camera
	if (first_person) 
		environment.light = camera_controller_first_person.camera_model.position(); //comment faire pour positionner la caméra à l'endroit souhaité ? 
	else
		environment.light = camera_control.camera_model.position();

	
	
	if (gui.display_frame)
		draw(global_frame, environment);
	// update coordinate vefore drawing
	// hierarchy.update_local_to_global_coordinates();

	// draw(hierarchy, environment);
	draw(maze, environment);
	draw(player, environment);

	if (gui.display_wireframe){
		draw_wireframe(maze, environment);
		draw_wireframe(player, environment);
	}

	// timer.update();
	//cout << timer.t << endl;
	//player.model.translation = timer.t * (camera_controller_first_person.camera_model.matrix_frame() * vec4(1, 0, 0, 0)).xyz(); 
	// player.model.rotation *= rotation_transform::from_axis_angle({ 0, 0, 1 }, Pi * timer.t);

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	ImGui::ColorEdit3("Light color", &gui.light_color[0]);
	ImGui::SliderFloat3("Light position", &gui.light_position[0], -3.0f, 3.0f);

	ImGui::SliderFloat("Ambiant", &gui.ambiant, 0.0f, 1.0f);
	ImGui::SliderFloat("Diffuse", &gui.diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular", &gui.specular, 0.0f, 1.0f);
	ImGui::SliderFloat("Specular_exp", &gui.specular_exp, 0, 256);
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
	
	float const magnitude = 2 * inputs.time_interval;
	float const angle_magnitude = 2 * inputs.time_interval;
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
	// 	player.model.translation += vec3( 0, magnitude, 0 );
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
		if (collision_detection()) {
			camera_controller_first_person.no_collision = false;// cout << "collision" << endl;
		}
	}
	else 
	 	camera_control.idle_frame(environment.camera_view);
		
}

bool scene_structure::collision_detection() {
	

	double up = std::round(player.model.translation.y) + 0.5f * (HEIGHT % 2);
	double left = std::round(player.model.translation.x) - 0.5f * (WIDTH % 2);
	
	int i = WIDTH / 2  + std::floor(player.model.translation.x - 0.5 * (WIDTH % 2));
	int j = HEIGHT / 2 - std::ceil(player.model.translation.y - 0.5 * (HEIGHT % 2));

	float d_min = 0.08f;
	// up collision
	if ((std::abs(player.model.translation.y - up) < d_min) && maze_test[i][j].walls[0])
		return true;
	// down collision
	if ((std::abs(player.model.translation.y - up - 1) < d_min) && maze_test[i][j].walls[1])
		return true;
	// left collision
	if ((std::abs(player.model.translation.x - left) < d_min) && maze_test[i][j].walls[2])
		return true;
	// right collision
	if ((std::abs(player.model.translation.x - left - 1) < d_min) && maze_test[i][j].walls[3])
		return true;
	cout << "no collision" << endl;
	return false;
}