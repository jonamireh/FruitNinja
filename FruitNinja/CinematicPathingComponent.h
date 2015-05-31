#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <functional>

class CinematicPathingComponent
{
public:
	CinematicPathingComponent(std::vector<glm::vec3> position_points, std::vector<glm::vec3> lookAt_points, float move_speed);
	CinematicPathingComponent() {}
	//first = position
	//second = lookAt
	std::pair<glm::vec3, glm::vec3> get_direction();
	bool done = false;
	//first = position
	//second = lookAt
	float get_move_speed();
	float get_lookat_speed();
private:
	std::vector<glm::vec3> position_points;
	std::vector<glm::vec3> lookAt_points;
	float move_speed;
	int current_curve;
	float current_distance;
	float current_time;
	float time_elapsed;
	void change_path();
	std::pair<glm::vec3, glm::vec3> get_rom_catmull_direction();
};