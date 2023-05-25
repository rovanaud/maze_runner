
#include "camera.hpp"


#include <cmath>

using namespace cgp;


void maze_camera_controller::action_mouse_move(mat4& camera_matrix_view)
{
	// Preconditions
	assert_cgp_no_msg(inputs != nullptr);
	assert_cgp_no_msg(window != nullptr);
	if (!is_active) return;

	vec2 const& p1 = inputs->mouse.position.current;
	vec2 const& p0 = inputs->mouse.position.previous;
	vec2 const dp = p1 - p0;

	bool const event_valid = !inputs->mouse.on_gui;
	bool const click_left = inputs->mouse.click.left;
	bool const click_right = inputs->mouse.click.right;


	if (event_valid) {
		if (!is_cursor_trapped) {
			if (click_left)
				camera_model.manipulator_rotate_roll_pitch_yaw(0, -dp.y, dp.x);
			else if (click_right)
				camera_model.manipulator_translate_front(-(p1 - p0).y);
		}
		else if (is_cursor_trapped)
			camera_model.manipulator_rotate_roll_pitch_yaw(0, -dp.y, dp.x);
	}

	camera_matrix_view = camera_model.matrix_view();
}

void maze_camera_controller::action_keyboard(mat4&)
{
	if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_C) && inputs->keyboard.shift) {
		is_cursor_trapped = !is_cursor_trapped;
		if (is_cursor_trapped)
			glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	// Escape also gives back the cursor
	if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_ESCAPE)) {
		is_cursor_trapped = false;
		glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void maze_camera_controller::idle_frame(mat4& camera_matrix_view)
{
	//cout << "here we are " << endl;
	
	// Preconditions
	assert_cgp_no_msg(inputs != nullptr);
	assert_cgp_no_msg(window != nullptr);
	if (!is_active) return;

	float const magnitude = inputs->time_interval;
	float const angle_magnitude = inputs->time_interval;


	// displacement with WSAD

	//   front/back
	if (inputs->keyboard.is_pressed(GLFW_KEY_W)) {
		//camera_model.manipulator_translate_front(-magnitude);
		vec3 new_position = camera_model.position_camera + camera_model.front() * magnitude;
		if (possible_move(new_position.x, new_position.y)) cout << true << endl;
		if (possible_move(new_position.x, new_position.y))
			camera_model.position_camera = new_position;
	}
	if (inputs->keyboard.is_pressed(GLFW_KEY_S)) {
		// camera_model.manipulator_translate_front(magnitude);
		vec3 new_position = camera_model.position_camera - camera_model.front() * magnitude;
		if (possible_move(new_position.x, new_position.y)) cout << true << endl;
		if (possible_move(new_position.x, new_position.y))
			camera_model.position_camera = new_position;
		//player->model.translation = distance * camera_model.front() + camera_model.position() - vec3(0, 0, 0.04f);
		//player->model.rotation = rotation_transform::rotation_transform(camera_model.orientation_camera);
	}
	//   twist // no need to check for collision
	if (no_collision && inputs->keyboard.is_pressed(GLFW_KEY_A)) {
		camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, -angle_magnitude);
		
		// if (possible_move(camera_model.position_camera.x, camera_model.position_camera.y)) cout << true << endl;
		// if (!possible_move(camera_model.position_camera.x, camera_model.position_camera.y))
		// 	camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, angle_magnitude);
		//player->model.translation = distance * camera_model.front() + camera_model.position() - vec3(0, 0, 0.04f);
		//player->model.rotation = rotation_transform::rotation_transform(camera_model.orientation_camera);
	}
	if (no_collision && inputs->keyboard.is_pressed(GLFW_KEY_D)) {
		camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, angle_magnitude);
		// cout << possible_move(camera_model.position_camera.x, camera_model.position_camera.y) << endl;
		// if (!possible_move(camera_model.position_camera.x, camera_model.position_camera.y))
		//	camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, -angle_magnitude);
		//player->model.translation = distance * camera_model.front() + camera_model.position() - vec3(0, 0, 0.04f);
		//player->model.rotation = rotation_transform::rotation_transform(camera_model.orientation_camera);
	}
	

	// With arrows
	if (inputs->keyboard.ctrl == false) { //update position player 
		if (inputs->keyboard.up)
			if (check_wall(player->model.translation.x - magnitude * cos(angle),
				player->model.translation.y - magnitude * sin(angle)))
			{
				camera_model.manipulator_translate_front(-magnitude);
				player->model.translation.x += -magnitude * cos(angle);
				player->model.translation.y += -magnitude * sin(angle);
			}
		if (inputs->keyboard.down)
			if (check_wall(player->model.translation.x + magnitude * cos(angle),
				player->model.translation.y + magnitude * sin(angle)))
			{
				camera_model.manipulator_translate_front(magnitude);
				player->model.translation.x += magnitude * cos(angle);
				player->model.translation.y += magnitude * sin(angle);
			}
		if (inputs->keyboard.left)
			camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, -angle_magnitude);
			if (angle >= 360)
			{
				angle = 360 - angle; 

			}
			else {
				angle += -angle_magnitude;
			}
		if (inputs->keyboard.right)
			camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, angle_magnitude);
			if (angle >= 360)
			{
				angle = 360 - angle;

			}
			else {
				angle += -angle_magnitude;
			}
	}
	else {
		if (inputs->keyboard.up)
			camera_model.manipulator_translate_front(-magnitude);
		if (inputs->keyboard.down)
			camera_model.manipulator_translate_front(magnitude);
		if (inputs->keyboard.left)
			camera_model.manipulator_rotate_roll_pitch_yaw(angle_magnitude, 0, 0);
		if (inputs->keyboard.right)
			camera_model.manipulator_rotate_roll_pitch_yaw(-angle_magnitude, 0, 0);
	}

	camera_matrix_view = camera_model.matrix_view();
}

void maze_camera_controller::set_player(mesh_drawable& _player) {
	distance = norm(camera_model.position() - _player.model.translation);
	std::cout << distance << std::endl;
	player = &_player;
}

bool maze_camera_controller::check_wall(float x, float y)
{
	int col = std::floor(x); // donne la pose de la col et rox dans le lab 
	int row = std::floor(y);

	if (x+0.5 >= row+0.5 && maze[row][col].walls[0]) //up
	{
		return true; 
	}
	else if (x - 0.5 <= row - 0.5 && maze[row][col].walls[1]) 
	{
		return true; 

	//}
	//else if (y + 0.5 >= col + 0.5 && maze[row][col].walls[3])
	//{
	//	return true; 
	//}
	//else if (y + 0.5 >= col + 0.5 && maze[row][col].walls[3])
	//{
	//	return true; 
	//}
	//else {
	//	return false; 
	//}
	return false;
};

void maze_camera_controller::set_maze(vector<vector<Cell>>& _maze_v, mesh_drawable& _maze_d) {
	maze_v = &_maze_v;
	maze_d = &_maze_d;
}

int maze_camera_controller::possible_move(float x, float y) {
	/*

		double up = std::round(player.model.translation.y) + 0.5f * (HEIGHT % 2);
		double left = std::round(player.model.translation.x) - 0.5f * (WIDTH % 2);

		int i = WIDTH / 2  + std::floor(player.model.translation.x - 0.5 * (WIDTH % 2));
		int j = HEIGHT / 2 - std::ceil(player.model.translation.y - 0.5 * (HEIGHT % 2));

		float d_min = 0.08f;
		// up collision
		if ((std::abs(player.model.translation.y - up) < d_min) && maze_v[i][j].walls[0])
			return true;
		// down collision
		if ((std::abs(player.model.translation.y - up - 1) < d_min) && maze_v[i][j].walls[1])
			return true;
		// left collision
		if ((std::abs(player.model.translation.x - left) < d_min) && maze_v[i][j].walls[2])
			return true;
		// right collision
		if ((std::abs(player.model.translation.x - left - 1) < d_min) && maze_v[i][j].walls[3])
			return true;
		cout << "no collision" << endl;
		return false;

	*/

	double w_half(0.5f * (WIDTH % 2));
	double h_half((HEIGHT % 2) * .5f);

	cout << "w_half  " << w_half << "  h_half  " << h_half << endl;

	double up = h_half + std::ceil(y - h_half); // smallest integer greater than or equal to y
	double left = w_half + std::floor(x - w_half); // largest integer less than or equal to x

	cout << "up  " << up << " left  " << left << endl;
	cout << "x  " << x << " y  " << y << endl;

	// C'est un peu compliqué ici parce que les indices de la matrices sont inversés...
	int i = std::round(HEIGHT / 2 -  up  + h_half); // round to the nearest integer
	int j = std::round(WIDTH  / 2 + left + w_half); // round to the nearest integer

	cout << "i  " << i << "  j  " << j << endl;

	float d_min = 0.05f; // epaisseur du mur / 2 plus un epsilon

	cout << "up  " << up - y << "  left  " << x - left << "  down  " << y - up + 1 << "  right  " << - x + left + 1 << endl;
	cout << "walls up " << (*maze_v)[i][j].walls[0] << " down " << (*maze_v)[i][j].walls[1] << " left " << (*maze_v)[i][j].walls[2] << " right " << (*maze_v)[i][j].walls[3] << endl;
	// up collision
	if ((up - y < d_min) && (*maze_v)[i][j].walls[0])
		return false;
	// down collision
	if ((y - up + 1 < d_min) && (*maze_v)[i][j].walls[1])
		return false;
	// left collision
	if ((x - left < d_min) && (*maze_v)[i][j].walls[2])
		return false;
	// right collision
	if ((left + 1 - x < d_min) && (*maze_v)[i][j].walls[3])
		return false;
	
	//cout << "no collision" << endl;
	return true;

	// return false;
}
