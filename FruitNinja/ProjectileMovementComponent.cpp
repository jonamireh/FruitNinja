#include "ProjectileMovementComponent.h"
#include "World.h"
#include "ProjectileEntity.h"

//#include <GLFW/glfw3.h>

void ProjectileMovementComponent::update()
{
	
	if (!entity.shot) {
		entity.position = camera->cameraPosition;
		velocity = camera->cameraFront * ARROW_SPEED;

		glm::vec3 look = glm::normalize(camera->cameraFront);
		glm::vec3 right = glm::cross(camera->cameraUp, look);
		glm::vec3 up2 = glm::cross(look, right);

		entity.rot[0] = glm::vec4(right, 0);
		entity.rot[1] = glm::vec4(up2, 0);
		entity.rot[2] = glm::vec4(look, 0);
		entity.rot[3] = glm::vec4(0, 0, 0, 1);
		entity.should_draw = true;
		entity.shot = true;
		std::cout << "Shot it too" << std::endl;
	}
	else {
		entity.position += velocity * (float)seconds_passed;
		entity.timeLeft -= seconds_passed;
		if (entity.timeLeft < 0) entity.should_draw = false;
	}
}