#include "scene.hpp"

#include "terrain.hpp"
#include "tree.hpp"
#include "draw_walls.hpp"

using namespace cgp;




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 5.0f,2.0f,2.0f }, {0,0,0});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	//int N_terrain_samples = 100;
	//float terrain_length = 20;
	//mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
	//terrain.initialize_data_on_gpu(terrain_mesh);
	//terrain.material.color = { 0.6f,0.85f,0.5f };
	//terrain.material.phong.specular = 0.0f; // non-specular terrain material

	//int const N_trees = 150;
	//tree_positions = generate_positions_on_terrain(N_trees, terrain_length);

	/*mesh const cylinder_mesh = create_cylinder_mesh(2.0f, 5.0f);
	cylinder.initialize_data_on_gpu(cylinder_mesh);
	cylinder.material.color = { 0.6f,0.85f,0.5f };
	cylinder.material.phong.specular = 0.0f;
	
	mesh const cone_mesh = create_cone_mesh(2.0f, 5.0f, 5.0f);
	cone.initialize_data_on_gpu(cone_mesh);
	cone.material.color = { 0.6f,0.85f,0.5f };
	cone.material.phong.specular = 0.0f;*/

	//mesh const tree_mesh = create_tree();
	//tree.initialize_data_on_gpu(tree_mesh);

	//mesh const mushroom_mesh = create_mushroom();
	//mushroom.initialize_data_on_gpu(mushroom_mesh);


	std::vector<std::vector<cgp::vec2>> test_set(4);
	
	vec2 p1 = vec2(1.0f, 0.0f);
	vec2 p2 = vec2(0.0f, 0.1f);
	float h = 1.0f;
	float e = .3f;

	test_set[0].push_back(vec2(0.0f, 0.0f));
	test_set[0].push_back(vec2(0.0f, 0.91f));

	test_set[1].push_back(vec2(0.0f, 0.91f));
	test_set[1].push_back(vec2(0.91f, 0.91f));

	test_set[2].push_back(vec2(0.91f, 0.91f));
	test_set[2].push_back(vec2(0.91f, 0.0f));

	test_set[3].push_back(vec2(0.91f, 0.0f));
	test_set[3].push_back(vec2(0.0f, 0.0f));

	mesh maze_mesh = create_maze(test_set, .8f, 0.1f); //draw_wall(p1, p2, h, e); 
	maze.initialize_data_on_gpu(maze_mesh);



}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	//draw(terrain, environment);
	draw(maze, environment);

	if (gui.display_wireframe){
		//draw_wireframe(terrain, environment);
		draw_wireframe(maze, environment);
	}

  /*
	draw(cylinder, environment);
	if (gui.display_wireframe)
		draw_wireframe(cylinder, environment);

	draw(cone, environment);
	if (gui.display_wireframe)
		draw_wireframe(cone, environment);
  */

	//int n = tree_positions.size(); 
	//int t = n / 3;

	//for (int i = 0; i < t; i++) {
	//	tree.model.translation = tree_positions[i] - vec3(0.0f, 0.0f, 0.0001f);
	//	draw(tree, environment);
	//	if (gui.display_wireframe)
	//		draw_wireframe(tree, environment);
	//}
	//for(int i = t; i < n; i++){
	//	mushroom.model.translation = tree_positions[i] - vec3(0.0f, 0.0f, 0.0001f);
	//	draw(mushroom, environment);
	//	if (gui.display_wireframe)
	//		draw_wireframe(mushroom, environment);
	//}

}


void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}

