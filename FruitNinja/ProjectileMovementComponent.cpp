#include "ProjectileMovementComponent.h"
#include "World.h"
#include "ProjectileEntity.h"

//#include <GLFW/glfw3.h>

void ProjectileMovementComponent::update()
{
	static glm::vec3 velocity(0.0f, 0.0f, 0.0f);
	if (keys[GLFW_KEY_E]) {
		entity.position = entity.owner->position;
		velocity = glm::normalize(camera->cameraFront) * ARROW_SPEED;
	}
	entity.position += velocity * (float)seconds_passed;
	//std::cout << entity.position.x << ", " << entity.position.y << ", " << entity.position.z << "\n";
}