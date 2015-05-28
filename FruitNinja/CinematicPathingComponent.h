#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <functional>

class CinematicPathingComponent
{
public:
	std::vector<glm::vec3> control_points;
	CinematicPathingComponent(std::vector<glm::vec3> control_points, float move_speed, void* reference, std::function<void(void*, float, float)> callback);
	CinematicPathingComponent() {}
	glm::vec3 get_direction();
	bool done = false;
	float get_move_speed();
private:
	float move_speed;
	int current_curve;
	float current_distance;
	float current_time;
	float time_elapsed;
	void change_path();
	void* reference;
	std::function<void(void*, float, float)> callback;
	glm::vec3 get_rom_catmull_direction();
};