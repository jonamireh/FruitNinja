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
	int current_curve;
	int max_curve;
private:
	std::vector<glm::vec3> position_points;
	std::vector<glm::vec3> lookAt_points;
	float move_speed;;
	float position_distance;
	float lookAt_distance;
	float position_time;
	float time_elapsed;
	void change_path();
	std::pair<glm::vec3, glm::vec3> get_rom_catmull_direction();
	glm::vec3 getPosition(float time, std::vector<glm::vec3> source);
	float get_distance(std::vector<glm::vec3> source);
};