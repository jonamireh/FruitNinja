#include "GameEntity.h"

#define PI 3.14159
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

using namespace glm;

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
        for (int j = 0; j < meshes[i]->data.size(); j++)
        {
            tempCenter += meshes[i]->data[j].pos;
            numVertices++;
            float temp = distance(meshes[i]->data[0].pos, meshes[i]->data[j].pos);
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

