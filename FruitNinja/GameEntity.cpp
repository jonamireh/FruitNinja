#include "GameEntity.h"

#define PI 3.14159

void GameEntity::turnTo(glm::vec3 cartesian) {
	if (cartesian.x < 0)
		rotations.y = -1.0f * glm::atan(cartesian.z / cartesian.x);
	else
		rotations.y = glm::atan(cartesian.z / -cartesian.x) + PI;
}