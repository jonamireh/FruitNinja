#include "PlatformMovementComponent.h"
#include "World.h"

using namespace glm;
using namespace std;

void PlatformMovementComponent::update(bool static_movement)
{
	if (!static_movement) {
		direction = pathing.get_direction();
		entity.setPosition(entity.getPosition() + seconds_passed * move_speed * direction);
	}
}