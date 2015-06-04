#include "GuardMovementComponent.h"
#include "World.h"

using namespace glm;
using namespace std;

void GuardMovementComponent::update(bool static_movement)
{
	if (!static_movement)
		direction = pathing.get_direction();
	float toAngle = entity.turnAngle(direction).y;
	vec3 rotations = entity.getRotations();
	float fromAngle = rotations.y;

	if (toAngle - fromAngle < -M_PI)
	{
		toAngle += 2 * M_PI;
		rotations.y += (toAngle - fromAngle) * GUARD_ROTATE_SPEED * seconds_passed;
		if (rotations.y > M_PI)
			rotations.y -= 2 * M_PI;
	}
	else if (toAngle - fromAngle > M_PI)
	{
		fromAngle += 2 * M_PI;
		rotations.y += (toAngle - fromAngle) * GUARD_ROTATE_SPEED * seconds_passed;
		if (rotations.y < -M_PI)
			rotations.y += 2 * M_PI;
	}
	else
		rotations.y += (toAngle - fromAngle) * GUARD_ROTATE_SPEED * seconds_passed;

	//vec3 pos_offset = direction * GUARD_MOVE_SPEED * seconds_passed;
	//entity.position += pos_offset;
	if (!static_movement)
		entity.setPosition(entity.getPosition() + seconds_passed * move_speed * direction);
	entity.setRotations(rotations);
}