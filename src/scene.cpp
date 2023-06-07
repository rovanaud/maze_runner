#include "scene.hpp"
#include "interpolation.hpp"

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

	// Definition of the initial data
	//--------------------------------------//

	// Key 3D positions
	vector<vec3> key_positions = generatePath3D(15, 1);

	cout << "size key_positions:" << key_positions.size() << endl; 

	// Key times (time at which the position must pass in the corresponding position)
	vector<float> key_times(key_positions.size());
	for (size_t i = 0; i < key_positions.size(); i++)
	{
		key_times[i] = (float)i + 1.0; 
	}

	// Initialize the helping structure to display/interact with these positions
	keyframe.initialize(key_positions, key_times);


	// Set timer bounds
	// The timer must span a time interval on which the interpolation can be conducted
	// By default, set the minimal time to be key_times[1], and the maximal time to be key_time[N-2] (enables cubic interpolation)
	int N = key_times.size();
	timer1.t_min = key_times[1];
	timer1.t_max = key_times[N - 2];
	timer1.t = timer1.t_min;
	timer1.scale = 0.040; 
	
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
		
	}
	else {
		camera_control.initialize(inputs, window);
		camera_control.look_at(vec3(0, 0, 20), vec3(0, 0, 0), vec3(0, 0, 1));
	}

	mazeMesh.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_grass.jpg",
		GL_MIRRORED_REPEAT,
		GL_MIRRORED_REPEAT);
}

void scene_structure::display_frame()
{
	//draw(sphere_light, environment);

	// Update the current time
	timer1.update();
	float t = timer1.t;

	for (int i = 0; i < 20; i++)
	{

		if (t < timer1.t_min + 0.1f)
			keyframe.trajectory.clear();

		// Display the key positions and lines b/w positions
		keyframe.display_key_positions(environment);

		// Compute the interpolated position
		//  This is this function that you need to complete
		vec3 p = interpolation(t+i*5, keyframe.key_positions, keyframe.key_times);

		// Display the interpolated position (and its trajectory)
		keyframe.display_current_position(p, environment);

	}

	float h = .50f;

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
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	ImGui::Checkbox("Frame", &gui.display_frame);

	ImGui::SliderFloat("Time", &timer.t, timer1.t_min, timer1.t_max);
	ImGui::SliderFloat("Time scale", &timer1.scale, 0.0f, 2.0f);

	// Display the GUI associated to the key position
	keyframe.display_gui();

}

void scene_structure::mouse_move_event()
{
	//if (!inputs.keyboard.shift)
		//camera_control.action_mouse_move(environment.camera_view);
	/*if (first_person)
		camera_controller_first_person.action_mouse_move(environment.camera_view);
	else
		camera_control.action_mouse_move(environment.camera_view);*/
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

	float r = 0.2f;
	
	
	float const magnitude = inputs.time_interval;
	float const angle_magnitude = inputs.time_interval;

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
	if (first_person){
		camera_controller_first_person.idle_frame(environment.camera_view);
	}
	else 
	 	camera_control.idle_frame(environment.camera_view);
		
}

vector <vec3> scene_structure :: generatePath3D(double squareSize, double minDistance) {

	double cellSize = minDistance / std::sqrt(2);
	int gridSize = static_cast<int>(std::ceil(squareSize / cellSize));

	std::vector<std::vector<vec2>> grid(gridSize, std::vector<vec2>(gridSize, { -1, -1 }));
	std::vector<vec3> points;
	std::vector<vec2> activeList;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(0.0, 1.0);

	auto isValidPoint = [&](const vec2& p) {
		int cellX = static_cast<int>(p[0] / cellSize);
		int cellY = static_cast<int>(p[1] / cellSize);

		int startX = std::max(0, cellX - 2);
		int startY = std::max(0, cellY - 2);
		int endX = std::min(cellX + 2, gridSize - 1);
		int endY = std::min(cellY + 2, gridSize - 1);

		for (int x = startX; x <= endX; ++x) {
			for (int y = startY; y <= endY; ++y) {
				if (grid[x][y].x != -1 && grid[x][y].y != -1) {
					double dx = grid[x][y].x - p.x;
					double dy = grid[x][y].y - p.y;
					double distanceSquared = dx * dx + dy * dy;

					if (distanceSquared < minDistance * minDistance) {
						return false;
					}
				}
			}
		}

		return true;
	};

	float dx = 15 / 2.0f;
	float dy = 15 / 2.0f;


	auto addPoint = [&](const vec2& p) {
		int cellX = static_cast<int>(p.x / cellSize);
		int cellY = static_cast<int>(p.y / cellSize);

		grid[cellX][cellY] = p;
		activeList.push_back(p);
		points.push_back({p.x - dx, p.y - dy, 0});
	};

	double initialX = dis(gen) * squareSize;
	double initialY = dis(gen) * squareSize;
	vec2 initialPoint = { initialX, initialY };
	addPoint(initialPoint);

	while (!activeList.empty()) {
		int index = std::uniform_int_distribution<int>(0, activeList.size() - 1)(gen);
		vec2 currentPoint = activeList[index];
		activeList.erase(activeList.begin() + index);

		for (int i = 0; i < 30; ++i) {
			double angle = 2 * 3.14 * dis(gen);
			double distance = minDistance + dis(gen) * minDistance;
			double newX = currentPoint.x + distance * std::cos(angle);
			double newY = currentPoint.y + distance * std::sin(angle);

			if (newX >= 0 && newX < squareSize && newY >= 0 && newY < squareSize) {
				vec2 newPoint = { newX, newY };

				if (isValidPoint(newPoint)) {
					addPoint(newPoint);
				}
			}
		}
	}

	return points;
}