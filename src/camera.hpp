#include "cgp/cgp.hpp"

using namespace cgp;
using namespace std;


float angle; 

struct maze_camera_controller : camera_controller_first_person
{
	camera_first_person camera_model;

	mesh_drawable *player;

	
	void action_mouse_move(mat4& camera_matrix_view);

	//void action_mouse_move(camera_orbit_spherical_coord& camera);
	//void action_mouse_move(camera_orbit& camera);

	// Key 'C' (in capital) to capture the cursor
	void action_keyboard(mat4& camera_matrix_view);// camera_generic_base& camera);

	// Keys up/down/left/right used to translate the camera
	//void idle_frame(camera_orbit_spherical_coord& camera);
	void idle_frame(mat4& camera_matrix_view);

	// set player
	void set_player(mesh_drawable& player);

private:
	bool is_cursor_trapped = false; // true = cursor captured (/infinite motion), false = cursor free
};


