#include "GuardPathingComponent.h"
#include <iostream>

using namespace std;
using namespace glm;

#define MIN_NUM_CONTROL_POINTS 4

GuardPathingComponent::GuardPathingComponent(vector<vec3> control_points, float animation_time) : control_points(control_points), animation_time(animation_time)
{
	assert(control_points.size() >= MIN_NUM_CONTROL_POINTS);
}


glm::vec3 GuardPathingComponent::getPosition(float dt)
{
	pair<int, float> info = getInterpolatedTime(dt);
	int curve_index = info.first;
	float time = info.second;

	assert(time <= 1.f);
	assert(curve_index <= control_points.size() - 2);

	cout << curve_index << endl;

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
}
glm::vec3 GuardPathingComponent::getDirection(float dt)
{
	pair<int, float> info = getInterpolatedTime(dt);
	int curve_index = info.first;
	float time = info.second;

	assert(time <= 1.f);
	assert(curve_index <= control_points.size() - 2);

	cout << curve_index << endl;

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
	f.x = -1.5 * pow(time, 2) + 2 * time + -0.5;
	f.y = 4.5 * pow(time, 2) + -5 * time;
	f.z = -4.5 * pow(time, 2) + 4 * time + 0.5;
	f.w = 1.5 * pow(time, 2) + -time;

	return normalize(f.x * points.at(0) + f.y * points.at(1) + f.z * points.at(2) + f.w * points.at(3));
}

pair<int, float> GuardPathingComponent::getInterpolatedTime(float dt)
{
	float temp = dt * (1 / (animation_time / (control_points.size() - 1)));
	return pair<int, float>(dt / (animation_time / (control_points.size() - 1)), temp - floor(temp));
}

void GuardPathingComponent::reverse()
{
	std::reverse(control_points.begin(), control_points.end());
}
