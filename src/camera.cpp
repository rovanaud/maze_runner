
#include "camera.hpp"

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
		//if (possible_move(new_position.x, new_position.y)) cout << true << endl;
		if (possible_move(new_position.x, new_position.y))
			camera_model.position_camera = new_position;
	}
	if (inputs->keyboard.is_pressed(GLFW_KEY_S)) {
		// camera_model.manipulator_translate_front(magnitude);
		vec3 new_position = camera_model.position_camera - camera_model.front() * magnitude;
		//if (possible_move(new_position.x, new_position.y)) cout << true << endl;
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
		if (inputs->keyboard.up) {
			vec3 new_position = camera_model.position_camera + camera_model.front() * magnitude;
			if (possible_move(new_position.x, new_position.y))
				camera_model.position_camera = new_position;
		}
		if (inputs->keyboard.down) {
			vec3 new_position = camera_model.position_camera - camera_model.front() * magnitude;
			if (possible_move(new_position.x, new_position.y))
				camera_model.position_camera = new_position;
		}
		if (inputs->keyboard.left)
			camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, -angle_magnitude);
		if (inputs->keyboard.right)
			camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, angle_magnitude);
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
//
//void maze_camera_controller::set_player(mesh_drawable& _player) {
//	distance = norm(camera_model.position() - _player.model.translation);
//	std::cout << distance << std::endl;
//	player = &_player;
//}

//
//void maze_camera_controller::set_maze(vector<vector<Cell>>& _maze_v, mesh_drawable& _maze_d) {
//	maze_v = &_maze_v;
//	maze_d = &_maze_d;
//}
