#include "ChewyMovementComponent.h"
#include <GLFW/glfw3.h>
#include "World.h"
#include "ChewyMovementComponent.h"
#include <iostream>

glm::vec3 forwardDirection(std::shared_ptr<Camera> camera)
{
	return  glm::normalize(camera->cameraFront);
}

glm::vec3 backDirection(std::shared_ptr<Camera> camera)
{
	return  glm::normalize(camera->cameraFront) * -1.0f;
}

glm::vec3 rightDirection(std::shared_ptr<Camera> camera)
{
	return  glm::normalize(glm::cross(camera->cameraFront, glm::vec3(0.0, 1.0, 0.0)));
}

glm::vec3 leftDirection(std::shared_ptr<Camera> camera)
{
	return glm::normalize(glm::cross(camera->cameraFront, glm::vec3(0.0, 1.0, 0.0))) * -1.0f;
}

void ChewyMovementComponent::update()
{
	if (camera->in_use) {
		if (keys[GLFW_KEY_W]) {
			entity.position.x += forwardDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += forwardDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
		if (keys[GLFW_KEY_S]) {
			entity.position.x += backDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += backDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
		if (keys[GLFW_KEY_A]) {
			entity.position.x += leftDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += leftDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
		if (keys[GLFW_KEY_D]) {
			entity.position.x += rightDirection(camera).x * CHEWY_MOVE_SPEED * (float)seconds_passed;
			entity.position.z += rightDirection(camera).z * CHEWY_MOVE_SPEED * (float)seconds_passed;
		}
	}
}