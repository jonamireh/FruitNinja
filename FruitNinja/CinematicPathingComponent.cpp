#include "CinematicPathingComponent.h"
#include <iostream>
#include "World.h"

using namespace std;
using namespace glm;

#define MIN_NUM_CONTROL_POINTS 4

CinematicPathingComponent::CinematicPathingComponent(vector<vec3> position_points, vector<vec3> lookAt_points, float move_speed) : position_points(position_points), 
	lookAt_points(lookAt_points), move_speed(move_speed)
{
	assert(position_points.size() >= MIN_NUM_CONTROL_POINTS);
	assert(lookAt_points.size() >= MIN_NUM_CONTROL_POINTS);
	assert(position_points.size() == lookAt_points.size());
	current_curve = 0;
	change_path();
}

pair<vec3, vec3> CinematicPathingComponent::get_direction()
{
	time_elapsed += seconds_passed;
	if (time_elapsed > current_time)
	{
		if ((current_curve + 2) > position_points.size() - 1)
		{
			done = true;
		}
		else
		{
			current_curve++;
			change_path();
		}
	}
	return get_rom_catmull_direction();
}

pair<vec3, vec3> CinematicPathingComponent::get_rom_catmull_direction()
{
	float time = time_elapsed * (1 / current_time);
	//assert(time <= current_time);
	assert(current_curve <= position_points.size() - 2);

	vector<vec3> c_position_points;
	vector<vec3> c_lookAt_points;
	if (current_curve == 0)
	{
		c_position_points.push_back(position_points.at(0));
		c_position_points.push_back(position_points.at(0));
		c_position_points.push_back(position_points.at(1));
		c_position_points.push_back(position_points.at(2));

		c_lookAt_points.push_back(lookAt_points.at(0));
		c_lookAt_points.push_back(lookAt_points.at(0));
		c_lookAt_points.push_back(lookAt_points.at(1));
		c_lookAt_points.push_back(lookAt_points.at(2));
	}
	else if (current_curve == position_points.size() - 2)
	{
		int last_index = position_points.size() - 1;

		c_position_points.push_back(position_points.at(last_index - 2));
		c_position_points.push_back(position_points.at(last_index - 1));
		c_position_points.push_back(position_points.at(last_index));
		c_position_points.push_back(position_points.at(last_index));

		c_lookAt_points.push_back(lookAt_points.at(last_index - 2));
		c_lookAt_points.push_back(lookAt_points.at(last_index - 1));
		c_lookAt_points.push_back(lookAt_points.at(last_index));
		c_lookAt_points.push_back(lookAt_points.at(last_index));
	}
	else
	{
		c_position_points.push_back(position_points.at(current_curve - 1));
		c_position_points.push_back(position_points.at(current_curve));
		c_position_points.push_back(position_points.at(current_curve + 1));
		c_position_points.push_back(position_points.at(current_curve + 2));

		c_lookAt_points.push_back(lookAt_points.at(current_curve - 1));
		c_lookAt_points.push_back(lookAt_points.at(current_curve));
		c_lookAt_points.push_back(lookAt_points.at(current_curve + 1));
		c_lookAt_points.push_back(lookAt_points.at(current_curve + 2));
	}

	vec4 f;
	f.x = -1.5 * pow(time, 2) + 2 * time + -0.5;
	f.y = 4.5 * pow(time, 2) + -5 * time;
	f.z = -4.5 * pow(time, 2) + 4 * time + 0.5;
	f.w = 1.5 * pow(time, 2) + -time;

	return pair<vec3, vec3>(normalize(f.x * c_position_points.at(0) + f.y * c_position_points.at(1) + f.z * c_position_points.at(2) + f.w * c_position_points.at(3)), 
		normalize(f.x * c_lookAt_points.at(0) + f.y * c_lookAt_points.at(1) + f.z * c_lookAt_points.at(2) + f.w * c_lookAt_points.at(3)));
}

float CinematicPathingComponent::get_move_speed()
{
	//quadratic interpolation for speed of the camera movement
	const float adjustment = 1.f;
	float x = (position_points.size() - 2) / 2.f;
	float k = move_speed / ((x + adjustment) * (x - (position_points.size() - 1 + adjustment)));
	if (k > 0.f)
		k *= -1.f;
	float result = k * (current_curve + adjustment) * (current_curve - (position_points.size() - 1 + adjustment));
	return result;
}

float CinematicPathingComponent::get_lookat_speed()
{
	//quadratic interpolation for speed of the lookAt point
	const float adjustment = 1.f;
	//y = k * (x-a)*(x-b)
	float time = time_elapsed;
	float a = adjustment;
	float b = current_time + adjustment;
	float dist = glm::distance(lookAt_points.at(current_curve), lookAt_points.at(current_curve + 1));
	float k = (6 * dist) / (current_time * (6 * a * b - 3 * a * current_time - 3 * b * current_time + 2 * pow(current_time, 2)));
	if (k > 0.f)
		k *= -1.f;
	//float result = k * (time + a) * (time - b);
	float result = dist / current_time;
	return result;
}

void CinematicPathingComponent::change_path()
{
	current_distance = glm::distance(position_points.at(current_curve), position_points.at(current_curve + 1));
	current_time = current_distance / get_move_speed();
	time_elapsed = 0.f;
}
