#include "ProjectileMovementComponent.h"
#include "World.h"
#include "ProjectileEntity.h"

//#include <GLFW/glfw3.h>

void ProjectileMovementComponent::update()
{
	
	if (!entity.shot) {
		entity.setPosition(camera->cameraPosition);
		velocity = camera->cameraFront * ARROW_SPEED;

		entity.list = SET_DRAW(entity.list);
		entity.shot = true;
	}
	else {
		velocity.y -= GRAVITY * (float)seconds_passed;
		entity.setPosition(entity.getPosition() + velocity * (float)seconds_passed);
	}

	glm::vec3 look = glm::normalize(velocity);
	glm::vec3 right = glm::cross(camera->cameraUp, look);
	glm::vec3 up2 = glm::cross(look, right);

	entity.rot[0] = glm::vec4(right, 0);
	entity.rot[1] = glm::vec4(up2, 0);
	entity.rot[2] = glm::vec4(look, 0);
	entity.rot[3] = glm::vec4(0, 0, 0, 1);
}