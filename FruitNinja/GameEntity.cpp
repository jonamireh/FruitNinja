#include "GameEntity.h"

#define PI 3.14159
#include <glm/gtc/matrix_transform.inl>

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