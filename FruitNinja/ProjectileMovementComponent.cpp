#include "ProjectileMovementComponent.h"
#include "World.h"
#include "ProjectileEntity.h"

//#include <GLFW/glfw3.h>

using namespace glm;

void ProjectileMovementComponent::update()
{	
	if (!entity.shot) {
		vec3 pos = camera->cameraPosition + 6.f * camera->cameraFront;
		vec3 left = glm::normalize(glm::cross(vec3(0.f, 1.f, 0.f), camera->cameraFront));
		vec3 offset = 0.45f * left;
		offset.y -= 0.5f;
		entity.last_position = entity.inner_bounding_box.center;
		pos += offset;
		entity.setPosition(pos);
		entity.inner_bounding_box.center = entity.bounding_box.center;
		velocity = camera->cameraFront * ARROW_SPEED * bow_strength;

		entity.list = SET_DRAW(entity.list);
		entity.shot = true;
	}
	else {
		entity.last_position = entity.inner_bounding_box.center;
		velocity.y -= GRAVITY * (float)seconds_passed;
		entity.setPosition(entity.getPosition() + velocity * (float)seconds_passed);
		entity.inner_bounding_box.center += entity.bounding_box.center;
	}

	glm::vec3 look = glm::normalize(velocity);
	glm::vec3 right = glm::cross(camera->cameraUp, look);
	glm::vec3 up2 = glm::cross(look, right);

	entity.rot[0] = glm::vec4(right, 0);
	entity.rot[1] = glm::vec4(up2, 0);
	entity.rot[2] = glm::vec4(look, 0);
	entity.rot[3] = glm::vec4(0, 0, 0, 1);
}

glm::vec3 ProjectileMovementComponent::getVelocity() {
	return velocity;
}