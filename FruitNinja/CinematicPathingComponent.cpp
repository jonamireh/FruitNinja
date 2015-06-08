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
	max_curve = position_points.size() - 2;
	change_path();
}

pair<vec3, vec3> CinematicPathingComponent::get_direction()
{
time_elapsed += seconds_passed;
//cout << "time required: " << current_time << endl;
//cout << time_elapsed << endl;
if (time_elapsed > position_time)
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

glm::vec3 CinematicPathingComponent::getPosition(float time, vector<vec3> source)
{
	vector<vec3> points;
	if (current_curve == 0)
	{
		points.push_back(source.at(0));
		points.push_back(source.at(0));
		points.push_back(source.at(1));
		points.push_back(source.at(2));

	}
	else if (current_curve == source.size() - 2)
	{
		int last_index = source.size() - 1;

		points.push_back(source.at(last_index - 2));
		points.push_back(source.at(last_index - 1));
		points.push_back(source.at(last_index));
		points.push_back(source.at(last_index));
	}
	else
	{
		points.push_back(source.at(current_curve - 1));
		points.push_back(source.at(current_curve));
		points.push_back(source.at(current_curve + 1));
		points.push_back(source.at(current_curve + 2));
	}

	vec4 f;
	f.x = -0.5 * pow(time, 3) + pow(time, 2) + -0.5 * time;
	f.y = 1.5 * pow(time, 3) + -2.5 * pow(time, 2) + 1;
	f.z = -1.5 * pow(time, 3) + 2 * pow(time, 2) + 0.5 * time;
	f.w = 0.5 * pow(time, 3) + -0.5 * pow(time, 2);

	return f.x * points.at(0) + f.y * points.at(1) + f.z * points.at(2) + f.w * points.at(3);
}

pair<vec3, vec3> CinematicPathingComponent::get_rom_catmull_direction()
{
	float time = time_elapsed * (1 / position_time);
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
	return lookAt_distance / position_time;
}

float CinematicPathingComponent::get_distance(vector<vec3> source)
{
	float distance = 0.f;
	float epsilon = 0.001f;
	float last_time = 0.f;
	for (float i = epsilon; i < 1.f; i += epsilon)
	{
		distance += glm::distance(getPosition(last_time, source), getPosition(i, source));
		last_time = i;
	}
	return distance;
}

void CinematicPathingComponent::change_path()
{
	position_distance = get_distance(position_points);
	lookAt_distance = get_distance(lookAt_points);
	position_time = position_distance / get_move_speed();
	time_elapsed = 0.f;
}
