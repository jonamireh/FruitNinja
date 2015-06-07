#include "GuardPathingComponent.h"
#include <iostream>
#include "World.h"

using namespace std;
using namespace glm;

#define MIN_NUM_CONTROL_POINTS 4

// precondition: if it isn't linear provide at least 4 control points
GuardPathingComponent::GuardPathingComponent(vector<vec3> control_points, float move_speed, bool linear_curve) : control_points(control_points), move_speed(move_speed), linear_curve(linear_curve)
{
	current_curve = 0;
	change_path();
}

glm::vec3 GuardPathingComponent::get_direction()
{
	time_elapsed += seconds_passed;
	if (time_elapsed > current_time)
	{
		if ((current_curve + 2) > control_points.size() - 1)
		{
			reverse();
		}
		else
		{
			current_curve++;
			change_path();
		}
	}

	if (linear_curve)
		return get_linear_direction();
	else
		return get_rom_catmull_direction();
}

glm::vec3 GuardPathingComponent::get_linear_direction()
{
	return normalize(control_points.at(current_curve + 1) - control_points.at(current_curve));
}

glm::vec3 GuardPathingComponent::get_rom_catmull_direction()
{
	float time = time_elapsed * (1 / current_time);
	//assert(time <= current_time);
	assert(current_curve <= control_points.size() - 2);

	vector<vec3> points;
	if (current_curve == 0)
	{
		points.push_back(control_points.at(0));
		points.push_back(control_points.at(0));
		points.push_back(control_points.at(1));
		points.push_back(control_points.at(2));
	}
	else if (current_curve == control_points.size() - 2)
	{
		int last_index = control_points.size() - 1;

		points.push_back(control_points.at(last_index - 2));
		points.push_back(control_points.at(last_index - 1));
		points.push_back(control_points.at(last_index));
		points.push_back(control_points.at(last_index));

	}
	else
	{
		points.push_back(control_points.at(current_curve - 1));
		points.push_back(control_points.at(current_curve));
		points.push_back(control_points.at(current_curve + 1));
		points.push_back(control_points.at(current_curve + 2));
	}

	vec4 f;
	f.x = -1.5 * pow(time, 2) + 2 * time + -0.5;
	f.y = 4.5 * pow(time, 2) + -5 * time;
	f.z = -4.5 * pow(time, 2) + 4 * time + 0.5;
	f.w = 1.5 * pow(time, 2) + -time;

	return normalize(f.x * points.at(0) + f.y * points.at(1) + f.z * points.at(2) + f.w * points.at(3));
}


void GuardPathingComponent::reverse()
{
	std::reverse(control_points.begin(), control_points.end());
	current_curve = 0;
	change_path();
}

void GuardPathingComponent::change_path()
{
	current_distance = glm::distance(control_points.at(current_curve), control_points.at(current_curve + 1));
	current_time = current_distance / move_speed;
	time_elapsed = 0.f;
}
