
#include "camera.hpp"
#include "generateMaze.hpp"

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
	// Preconditions
	assert_cgp_no_msg(inputs != nullptr);
	assert_cgp_no_msg(window != nullptr);
	if (!is_active) return;

	float const magnitude = 2 * inputs->time_interval;
	float const angle_magnitude = 2 * inputs->time_interval;


	// displacement with WSAD

	//   front/back
	if (inputs->keyboard.is_pressed(GLFW_KEY_W))
		camera_model.manipulator_translate_front(-magnitude);
	if (inputs->keyboard.is_pressed(GLFW_KEY_S))
		camera_model.manipulator_translate_front(magnitude);
	//   twist
	if (inputs->keyboard.is_pressed(GLFW_KEY_A))
		camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, -angle_magnitude); 
	if (inputs->keyboard.is_pressed(GLFW_KEY_D))
		camera_model.manipulator_rotate_roll_pitch_yaw(0, 0, angle_magnitude);


	// With arrows
	if (inputs->keyboard.ctrl == false) { //update position player 
		if (inputs->keyboard.up)
			if (check_wall(player1.x - magnitude * cos(angle),
				player1.y - magnitude * sin(angle)))
			{
				camera_model.manipulator_translate_front(-magnitude);
				player1.x += -magnitude * cos(angle);
				player1.y += -magnitude * sin(angle);
			}
		if (inputs->keyboard.down)
			if (check_wall(player1.x + magnitude * cos(angle),
				player1.y + magnitude * sin(angle)))
			{
				camera_model.manipulator_translate_front(-magnitude);
				player1.x += magnitude * cos(angle);
				player1.y += magnitude * sin(angle);
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
	/*else {
		if (inputs->keyboard.up)
			camera_model.manipulator_translate_front(-magnitude);
		if (inputs->keyboard.down)
			camera_model.manipulator_translate_front(magnitude);
		if (inputs->keyboard.left)
			camera_model.manipulator_rotate_roll_pitch_yaw(angle_magnitude, 0, 0);
		if (inputs->keyboard.right)
			camera_model.manipulator_rotate_roll_pitch_yaw(-angle_magnitude, 0, 0);
	}*/


	camera_matrix_view = camera_model.matrix_view();
}
