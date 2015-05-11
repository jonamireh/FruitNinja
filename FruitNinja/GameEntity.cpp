#include "GameEntity.h"

#define PI 3.14159
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/detail/func_vector_relational.hpp>
#include "World.h"

using namespace glm;
using namespace std;


vec3 GameEntity::turnAngle(vec3 cartesian) {
	vec3 rot_angles(0, 0, 0);

	if (cartesian.x < 0)
		rot_angles.y = -1.0f * atan(cartesian.z / cartesian.x);
	else
		rot_angles.y = atan(cartesian.z / -cartesian.x) + PI;

    rot_angles.y -= PI / 2.f;

	return rot_angles;
}

void GameEntity::sebInit()
{
	shared_ptr<BoundingBox> outer_bb = getOuterBoundingBox();
    radius = glm::distance(outer_bb->lower_bound, outer_bb->upper_bound) / 2.f;
    center = (outer_bb->lower_bound + outer_bb->upper_bound)  / 2.f;
}

float GameEntity::getRadius()
{
    return scale * radius;
}

vec3 GameEntity::getCenter()
{
	shared_ptr<BoundingBox> outer_bb = getOuterBoundingBox();
	return vec3(center + position) + ((scale != 1.f) ? vec3(0.f, glm::distance(outer_bb->lower_bound, vec3(outer_bb->lower_bound.x, outer_bb->lower_bound.y, outer_bb->upper_bound.z)) / 2.f, 0.f) : vec3(0.f));
}

shared_ptr<BoundingBox> GameEntity::getOuterBoundingBox()
{
	if (largestBB == nullptr)
	{
		vector<Mesh*> meshes = mesh->getMeshes();
		vec3 lower_bound(FLT_MAX, FLT_MAX, FLT_MAX);
		vec3 upper_bound(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		for (int i = 0; i < meshes.size(); i++)
		{
			vec3 m_lower_bound(meshes.at(i)->getBoundingBox()->lower_bound);
			vec3 m_upper_bound(meshes.at(i)->getBoundingBox()->upper_bound);

			vec3 less = lessThan(m_lower_bound, lower_bound);
			vec3 greater = greaterThan(m_upper_bound, upper_bound);
			lower_bound.x = less.x ? m_lower_bound.x : lower_bound.x;
			lower_bound.y = less.y ? m_lower_bound.y : lower_bound.y;
			lower_bound.z = less.z ? m_lower_bound.z : lower_bound.z;
			upper_bound.x = greater.x ? m_upper_bound.x : upper_bound.x;
			upper_bound.y = greater.y ? m_upper_bound.y : upper_bound.y;
			upper_bound.z = greater.z ? m_upper_bound.z : upper_bound.z;
		}
		largestBB = shared_ptr<BoundingBox>(new BoundingBox(vec3(vec4(lower_bound, 1.0f)), vec3(vec4(upper_bound, 1.0f))));
	}
	return largestBB;
}

shared_ptr<BoundingBox> GameEntity::getTransformedOuterBoundingBox()
{
	shared_ptr<BoundingBox> obb = getOuterBoundingBox();
	
	vec3 lower_bound(obb->lower_bound);
	vec3 upper_bound(obb->upper_bound);
    vector<vec3> points = obb->get_points();

	vector<vec3> transformed_points;
	mat4 model = getModelMat();
	
	for (int i = 0; i < points.size(); i++)
	{
		transformed_points.push_back(vec3(model * vec4(points.at(i), 1.f)));
	}
	vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < transformed_points.size(); i++)
	{
		if (transformed_points.at(i).x < min.x) min.x = transformed_points.at(i).x;
		if (transformed_points.at(i).x > max.x) max.x = transformed_points.at(i).x;
		if (transformed_points.at(i).y < min.y) min.y = transformed_points.at(i).y;
		if (transformed_points.at(i).y > max.y) max.y = transformed_points.at(i).y;
		if (transformed_points.at(i).z < min.z) min.z = transformed_points.at(i).z;
		if (transformed_points.at(i).z > max.z) max.z = transformed_points.at(i).z;
	}

	return shared_ptr<BoundingBox>(new  BoundingBox(min, max));
}

bool GameEntity::compare(std::shared_ptr<GameEntity> ge)
{
    bool return_value = false;

    std::shared_ptr<BoundingBox> my_bb = getTransformedOuterBoundingBox();
    std::shared_ptr<BoundingBox> their_bb = ge->getTransformedOuterBoundingBox();
	
    vector<vec3> my_points = my_bb->get_points();
    vector<vec3> their_points = their_bb->get_points();

    for (int i = 0; i < my_points.size(); i++)
    {
        if (their_bb->contains(my_points.at(i)) || my_bb->contains(their_points.at(i)))
            return_value = true;
    }

    return return_value;
}

void GameEntity::collisionBB(std::shared_ptr<GameEntity> entity)
{
    //cout << "generic object collided!" << endl;
}

void GameEntity::collisionBS(std::shared_ptr<GameEntity> entity)
{
	
}


glm::mat4 GameEntity::getModelMat()
{
    mat4 model_trans = translate(mat4(1.0f), position);
    mat4 model_rot_x = rotate(mat4(1.0f), rotations.x, vec3(1.f, 0.f, 0.f));
    mat4 model_rot_y = rotate(mat4(1.0f), rotations.y, vec3(0.f, 1.f, 0.f));
    mat4 model_rot_z = rotate(mat4(1.0f), rotations.z, vec3(0.f, 0.f, 1.f));
    mat4 model_scale = glm::scale(mat4(1.0f), vec3(scale, scale, scale));

    return model_trans * model_rot_z * model_rot_x * model_rot_y * model_scale;
}