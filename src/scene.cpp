#include "scene.hpp"
#include "generateMaze.hpp"
#include "terrain.hpp"
#include "tree.hpp"
#include "draw_maze.hpp"


using namespace cgp;
using namespace std;


void scene_structure::initialize()
{
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());
	
	std::vector<std::vector<cgp::vec2>> test_set(4);
	vec2 p1 = vec2(1.0f, 0.0f);
	vec2 p2 = vec2(0.0f, 0.1f);
	float h = 1.0f;
	float e = .3f;
	float r = 0.2f;
	
	
	// test de la fonction maze_into_connected_points
	Cell maze_test[HEIGHT][WIDTH];
	initMaze(maze_test);
	generateMaze(maze_test);
	printMaze(maze_test);

	auto x = maze_into_connected_points(maze_test);

	//for (auto e : x) {
	//	for (auto f : e) {
	//		for (auto g : f)
	//			std::cout << g << " ";
	//		std::cout << "\n";
	//	}
	//	std::cout << "-----------------------------" << std::endl;
	//}

	test_set[0].push_back(vec2(0.0f, 0.0f));
	test_set[0].push_back(vec2(0.0f, 0.91f));
	test_set[1].push_back(vec2(0.0f, 0.91f));
	test_set[1].push_back(vec2(0.91f, 0.91f));
	test_set[2].push_back(vec2(0.91f, 0.91f));
	test_set[2].push_back(vec2(0.91f, 0.0f));
	test_set[3].push_back(vec2(0.91f, 0.0f));
	test_set[3].push_back(vec2(0.0f, 0.0f));

	vector<vector<vec2>> y; // vecteur de vecteur de vecteur de doubles

	// Parcourir chaque élément du vecteur d'entiers et les convertir en double
	for (int i = 0; i < x.size(); i++) {
		vector<vec2> inner_vect_double;
		for (int j = 0; j < x[i].size(); j++) {
			vec2 inner_inner_vect_double(x[i][j][0], x[i][j][1]);
			inner_vect_double.push_back(inner_inner_vect_double);
		}
		y.push_back(inner_vect_double);
	}


	mesh maze_mesh = create_maze(y, .4f, 0.01f); //draw_wall(p1, p2, h, e); 
	
	maze.initialize_data_on_gpu(maze_mesh);
	maze.material.color = vec3({ 0.2f, 0.9f, 0.7f });
	maze.model.translation += vec3(-1, -1, 0);
	
	vec3 player_G = vec3(-1.5f, -1.5f, r / 4);
	vec3 xx = player_G + vec3(r, 0, 0);
	vec3 yy = player_G + vec3(0, r, 0);
	
	vec3 up = player_G +  1.3f * vec3(r * sqrt(2), r * sqrt(2), 0);

	mesh direction = mesh_primitive_triangle(yy, xx, up);
	direction.color.fill({ 1.f, 0, 0});
	
	mesh player_mesh = mesh_primitive_sphere(r, player_G);
	player_mesh.color.fill({ 0.9f, 0.8f, 0.1f });
	player_mesh.push_back(direction);
	
	player.initialize_data_on_gpu(player_mesh);


	// test for transparency
	// player.material.alpha = 0.1f;
	// player.material.color = { 1, 1, 1 };
	// player.model.translation += vec3(-1, -1, r / 4);

	camera_controller_first_person.camera_model.front();

	cout << "material alpha " << player.material.alpha << endl;
	
	// hierarchy.add(maze, "maze");
	// hierarchy.add(player, "player", "maze", { r, r, h/2 - r});

	// Finally setup the camera
	first_person = true;
	if (first_person) {
		camera_controller_first_person.initialize(inputs, window);
		camera_controller_first_person.set_player(player);

		// look_at( yeux, cible, normale) => front = cible - yeux 
		vec3 center = player_G + vec3(r * sqrt(2), r * sqrt(2), 0);
		vec3 eye = up;
		vec3 zaxis(0, 0, 1);
		camera_controller_first_person.camera_model.look_at(eye, center, zaxis);
		cout << "Comparison between camera model position and eye, center, up : " << endl;
		cout << camera_controller_first_person.camera_model.position() << endl;
		cout << eye << endl;
		cout << center << endl;
		player.initialize_data_on_gpu(mesh_primitive_arrow());
		player.model.translation = center;
		player.model.rotation = rotation_transform::rotation_transform(camera_controller_first_person.camera_model.orientation_camera);
		// J'ai loupé la formule à faire yeux - cible ce qui explique le moins...
		// cout << "excepted front : " << normalize(vec3(r * cos(Pi / 4), r * sin(Pi / 4), r / 2) - vec3(0, 0, r / 2)) << endl;
		// cout << "computed front : " << -(camera_controller_first_person.camera_model.matrix_frame() * vec4(0, 0, 1, 0)) << endl;
		// faire -(camera_controller_first_person.camera_model.matrix_frame() * vec4( 0, 0, 1, 0)).xyz() pour avoir un vec3 à la place d'un vec4

	}
	else {
		camera_control.initialize(inputs, window);
		camera_control.set_rotation_axis_z();
		camera_control.look_at(vec3(0, 0, 10), vec3(0, 0, 0), vec3(0, 0, 1));
	}
	
}

void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	if (first_person) 
		environment.light = camera_controller_first_person.camera_model.position();
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
	if(first_person) 
		camera_controller_first_person.idle_frame(environment.camera_view);
	else 
	 	camera_control.idle_frame(environment.camera_view);
		
}

/*
* On peut déplacer le joueur en utilisant les touches directionnelles et ensuite et seulement ensuite on peut placer la camera 
*/