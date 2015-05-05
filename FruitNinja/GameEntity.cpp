#include "GameEntity.h"

#define PI 3.14159
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/detail/func_vector_relational.hpp>

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
    std::vector<Mesh*> meshes = mesh->getMeshes();
    float largestDistance = -FLT_MAX;
    int numVertices = 0;
    vec3 tempCenter;
    for (int i = 0; i < meshes.size(); i++)
    {
        for (int j = 0; j < meshes[i]->verts.size(); j++)
        {
            tempCenter += meshes[i]->verts[j];
            numVertices++;
            float temp = glm::distance(meshes[i]->verts[0], meshes[i]->verts[j]);
            if (temp > largestDistance)
            {
                largestDistance = temp;
            }
        }
    }
    radius = largestDistance / 2.0f;
    center = tempCenter / (float) numVertices;
}

float GameEntity::getRadius()
{
    return scale * radius;
}

vec3 GameEntity::getCenter()
{
    return center + position;
}

shared_ptr<vector<BoundingBox>> GameEntity::getBoundingBoxes()
{
    shared_ptr<vector<BoundingBox>> toReturn = shared_ptr<vector<BoundingBox>>(new vector<BoundingBox>);

    vector<Mesh*> meshes = mesh->getMeshes();
    for (int i = 0; i < meshes.size(); i++)
    {
        BoundingBox newBB(vec3(vec4(meshes.at(i)->getBoundingBox()->lower_bound, 1.f)), vec3(vec4(meshes.at(i)->getBoundingBox()->upper_bound, 1.f)));

        toReturn->push_back(newBB);
    }
    return toReturn;
}

shared_ptr<BoundingBox> GameEntity::getOuterBoundingBox()
{
	if (largestBB == nullptr)
	{
		vector<Mesh*> meshes = mesh->getMeshes();
		vec3 upper_bound(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		vec3 lower_bound(FLT_MAX, FLT_MAX, FLT_MAX);
		for (int i = 0; i < meshes.size(); i++)
		{
			vec3 m_lower_bound = meshes.at(i)->getBoundingBox()->lower_bound;
			vec3 m_upper_bound = meshes.at(i)->getBoundingBox()->upper_bound;

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
	shared_ptr<BoundingBox> bb((std::make_shared<BoundingBox>(getOuterBoundingBox()->lower_bound, getOuterBoundingBox()->upper_bound)));
	bb->applyTransformedBounds(getModelMat());

	return bb;
}

bool GameEntity::compare(std::shared_ptr<GameEntity> ge)
{
    std::shared_ptr<BoundingBox> my_bb = getTransformedOuterBoundingBox();
    std::shared_ptr<BoundingBox> their_bb = ge->getTransformedOuterBoundingBox();
	
	if (my_bb->upper_bound.x > their_bb->lower_bound.x &&
			my_bb->lower_bound.x < their_bb->upper_bound.x &&
			my_bb->upper_bound.y > their_bb->lower_bound.y &&
			my_bb->lower_bound.y < their_bb->upper_bound.y &&
			my_bb->upper_bound.z > their_bb->lower_bound.z &&
			my_bb->lower_bound.z < their_bb->upper_bound.z)
	{
		return true;
	}
	return false;
    /*for (int i = 0; i < my_bb->size(); i++)
    {
        for (int j = 0; j < their_bb->size(); j++)
        {
            if (my_bb->upper_bound.x > their_bb->lower_bound.x &&
                my_bb->lower_bound.x < their_bb->upper_bound.x &&
                my_bb->upper_bound.y > their_bb->lower_bound.y &&
                my_bb->lower_bound.y < their_bb->upper_bound.y &&
                my_bb->upper_bound.z > their_bb->lower_bound.z &&
                my_bb->lower_bound.z < their_bb->upper_bound.z)
            {
                return true;
            }
        }
    }
    return false;*/
}

void GameEntity::collision(std::shared_ptr<BoundingBox> bb)
{
    //cout << "generic object collided!" << endl;
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