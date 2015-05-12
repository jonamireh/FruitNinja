#include "GuardMovementComponent.h"
#include "World.h"

using namespace glm;
using namespace std;

void GuardMovementComponent::update()
{
	time_elapsed += seconds_passed;
	if (animation_time > 0.f && time_elapsed > animation_time)
	{
		time_elapsed = seconds_passed;
		pathing.reverse();
	}
	if (animation_time > 0.f)
	{
		direction = pathing.getDirection(time_elapsed);
		float toAngle = entity.turnAngle(direction).y;
		float fromAngle = entity.rotations.y;

		if (toAngle - fromAngle < -M_PI)
		{
			toAngle += 2 * M_PI;
			entity.rotations.y += (toAngle - fromAngle) * GUARD_ROTATE_SPEED * seconds_passed;
			if (entity.rotations.y > M_PI)
				entity.rotations.y -= 2 * M_PI;
		}
		else if (toAngle - fromAngle > M_PI)
		{
			fromAngle += 2 * M_PI;
			entity.rotations.y += (toAngle - fromAngle) * GUARD_ROTATE_SPEED * seconds_passed;
			if (entity.rotations.y < -M_PI)
				entity.rotations.y += 2 * M_PI;
		}
		else
			entity.rotations.y += (toAngle - fromAngle) * GUARD_ROTATE_SPEED * seconds_passed;

		vec3 pos_offset = direction * GUARD_MOVE_SPEED * seconds_passed;
		entity.position += pos_offset;
	}
}