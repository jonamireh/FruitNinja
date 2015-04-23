#include "ChewyMovementComponent.h"
#include <GLFW/glfw3.h>
#include "World.h"
#include "ChewyMovementComponent.h"
#include <iostream>

using namespace glm;
using namespace std;

vec3 forwardDirection(std::shared_ptr<Camera> camera)
{
	return  normalize(camera->cameraFront);
}

vec3 backDirection(std::shared_ptr<Camera> camera)
{
	return  normalize(camera->cameraFront) * -1.0f;
}

vec3 rightDirection(std::shared_ptr<Camera> camera)
{
	return  normalize(cross(camera->cameraFront, vec3(0.0, 1.0, 0.0)));
}

vec3 leftDirection(std::shared_ptr<Camera> camera)
{
	return normalize(cross(camera->cameraFront, vec3(0.0, 1.0, 0.0))) * -1.0f;
}

void ChewyMovementComponent::update()
{
	if (camera->in_use) {
		if (keys[GLFW_KEY_W]) {
            entity.turnTo(camera->cameraFront);
			entity.position.x += forwardDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += forwardDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
		if (keys[GLFW_KEY_S]) {
            entity.turnTo(camera->cameraFront);
            entity.rotations.y += M_PI;
			entity.position.x += backDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += backDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
		if (keys[GLFW_KEY_A]) {
            entity.turnTo(camera->cameraFront);
            entity.rotations.y += M_PI_2;
			entity.position.x += leftDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += leftDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
		if (keys[GLFW_KEY_D]) {
            entity.turnTo(camera->cameraFront);
            entity.rotations.y -= M_PI_2;
			entity.position.x += rightDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += rightDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
	}
}