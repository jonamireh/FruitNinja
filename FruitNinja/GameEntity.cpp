#include "GameEntity.h"

#define PI 3.14159
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

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
    float largestDistance = FLT_MIN;
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
    mat4 model_trans = translate(mat4(1.0f), position);
    mat4 model_rot_x = rotate(mat4(1.0f), rotations.x, vec3(1.f, 0.f, 0.f));
    mat4 model_rot_y = rotate(mat4(1.0f), rotations.y, vec3(0.f, 1.f, 0.f));
    mat4 model_rot_z = rotate(mat4(1.0f), rotations.z, vec3(0.f, 0.f, 1.f));
    mat4 model_scale = glm::scale(mat4(1.0f), vec3(scale, scale, scale));
    for (int i = 0; i < meshes.size(); i++)
    {
        BoundingBox newBB(vec3(vec4(meshes.at(i)->getBoundingBox()->lower_bound, 1.f) * model_trans * model_rot_z * model_rot_x * model_rot_y * model_scale),
            vec3(vec4(meshes.at(i)->getBoundingBox()->upper_bound, 1.f) * model_trans * model_rot_z * model_rot_x * model_rot_y * model_scale));

        toReturn->push_back(newBB);
    }
    return toReturn;
}

bool GameEntity::compare(std::shared_ptr<GameEntity> ge)
{
    std::shared_ptr<std::vector<BoundingBox>> my_bb = getBoundingBoxes();
    std::shared_ptr<std::vector<BoundingBox>> their_bb = ge->getBoundingBoxes();

    for (int i = 0; i < my_bb->size(); i++)
    {
        for (int j = 0; j < their_bb->size(); j++)
        {
            if (my_bb->at(i).upper_bound.x > their_bb->at(j).lower_bound.x &&
                my_bb->at(i).lower_bound.x < their_bb->at(j).upper_bound.x &&
                my_bb->at(i).upper_bound.y > their_bb->at(j).lower_bound.y &&
                my_bb->at(i).lower_bound.y < their_bb->at(j).upper_bound.y &&
                my_bb->at(i).upper_bound.z > their_bb->at(j).lower_bound.z &&
                my_bb->at(i).lower_bound.z < their_bb->at(j).upper_bound.z)
            {
                return true;
            }
        }
    }
    return false;
}
