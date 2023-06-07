#include "cgp/cgp.hpp"
#include "util.hpp"

using namespace cgp;
using namespace std;

//struct Player {
//	int x; 
//	int y; 
//};

struct maze_camera_controller : camera_controller_first_person
{
	camera_first_person camera_model;

	bool no_collision = true;

	float distance = 0.5f;

	
	void action_mouse_move(mat4& camera_matrix_view);
	void action_keyboard(mat4& camera_matrix_view);// camera_generic_base& camera);

	void idle_frame(mat4& camera_matrix_view);


private:
	bool is_cursor_trapped = false; // true = cursor captured (/infinite motion), false = cursor free
	float angle;
};


