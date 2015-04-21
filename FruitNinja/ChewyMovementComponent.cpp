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
	if (keys[GLFW_KEY_W]) {
		entity.position += forwardDirection(camera) * CHEWY_MOVE_SPEED * (float)seconds_passed;
	}
	if (keys[GLFW_KEY_S]) {
		entity.position += backDirection(camera) * CHEWY_MOVE_SPEED * (float)seconds_passed;
	}
	if (keys[GLFW_KEY_A]) {
		entity.position += leftDirection(camera) * CHEWY_MOVE_SPEED * (float)seconds_passed;
	}
	if (keys[GLFW_KEY_D]) {
		entity.position += rightDirection(camera) * CHEWY_MOVE_SPEED * (float)seconds_passed;
	}
}