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
    vec3 normalized_camera_front = normalize(camera->cameraFront);
    float angle_to_front = dot(vec2(normalized_camera_front.x, normalized_camera_front.z), vec2(1.f, 0.f));
    if (normalized_camera_front.z < 0.f)
    {
        angle_to_front *= -1.f;
    }

	if (camera->in_use) {
		if (keys[GLFW_KEY_W]) {
            entity.rotations.y = angle_to_front;
			entity.position.x += forwardDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += forwardDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
		if (keys[GLFW_KEY_S]) {
            entity.rotations.y = angle_to_front + M_PI;
			entity.position.x += backDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += backDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
		if (keys[GLFW_KEY_A]) {
            entity.rotations.y = angle_to_front + M_PI_2;
			entity.position.x += leftDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += leftDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
		if (keys[GLFW_KEY_D]) {
            entity.rotations.y = angle_to_front - M_PI_2;
			entity.position.x += rightDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += rightDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
	}
}