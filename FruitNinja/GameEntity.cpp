#include "GameEntity.h"

#define PI 3.14159
#include <glm/gtc/matrix_transform.inl>

using namespace glm;

void GameEntity::turnTo(glm::vec3 cartesian) {
	if (cartesian.x < 0)
		rotations.y = -1.0f * glm::atan(cartesian.z / cartesian.x);
	else
		rotations.y = glm::atan(cartesian.z / -cartesian.x) + PI;

    rotations.y -= PI / 2.f;
}