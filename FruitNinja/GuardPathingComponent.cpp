#include "GuardPathingComponent.h"
#include <iostream>
#include "World.h"

using namespace std;
using namespace glm;

#define MIN_NUM_CONTROL_POINTS 4

GuardPathingComponent::GuardPathingComponent(vector<vec3> control_points, float move_speed) : control_points(control_points), move_speed(move_speed)
{
	assert(control_points.size() >= MIN_NUM_CONTROL_POINTS);
	current_curve = 0;
	change_path();
}


/*glm::vec3 GuardPathingComponent::getPosition(float dt)
{
	pair<int, float> info = getInterpolatedTime(dt);
	int curve_index = info.first;
	float time = info.second;

	assert(time <= 1.f);
	assert(curve_index <= control_points.size() - 2);

	vector<vec3> points;
	if (curve_index == 0)
	{
		points.push_back(control_points.at(0));
		points.push_back(control_points.at(0));
		points.push_back(control_points.at(1));
		points.push_back(control_points.at(2));
	}
	else if (curve_index == control_points.size() - 2)
	{
		int last_index = control_points.size() - 1;

		points.push_back(control_points.at(last_index - 2));
		points.push_back(control_points.at(last_index - 1));
		points.push_back(control_points.at(last_index));
		points.push_back(control_points.at(last_index));

	}
	else
	{
		--curve_index;
		points.push_back(control_points.at(curve_index));
		points.push_back(control_points.at(curve_index + 1));
		points.push_back(control_points.at(curve_index + 2));
		points.push_back(control_points.at(curve_index + 3));
	}

	vec4 f;
	f.x = -0.5 * pow(time, 3) + pow(time, 2) + -0.5 * time;
	f.y = 1.5 * pow(time, 3) + -2.5 * pow(time, 2) + 1;
	f.z = -1.5 * pow(time, 3) + 2 * pow(time, 2) + 0.5 * time;
	f.w = 0.5 * pow(time, 3) + -0.5 * pow(time, 2);

	return f.x * points.at(0) + f.y * points.at(1) + f.z * points.at(2) + f.w * points.at(3);
}*/

glm::vec3 GuardPathingComponent::getDirection()
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
