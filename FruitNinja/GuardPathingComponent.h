#pragma once
#include <glm/glm.hpp>
#include <vector>


class GuardPathingComponent
{
public:
	std::vector<glm::vec3> control_points;
	float animation_time;

	GuardPathingComponent(std::vector<glm::vec3> control_points, float animation_time);

	glm::vec3 getPosition(float dt);
	glm::vec3 getDirection(float dt);

	void reverse();

	std::pair<int, float> getInterpolatedTime(float dt);
};