#pragma once
#include <glm/glm.hpp>
#include <vector>


class GuardPathingComponent
{
public:
	std::vector<glm::vec3> control_points;

	GuardPathingComponent(std::vector<glm::vec3> control_points, float move_speed);

	glm::vec3 getDirection();

	void reverse();
private:
	int current_curve;
	float current_distance;
	float move_speed;
	float current_time;
	float time_elapsed;
	void change_path();
};