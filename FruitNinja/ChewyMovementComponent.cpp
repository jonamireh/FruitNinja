#include "ChewyMovementComponent.h"
#include <GLFW/glfw3.h>
#include "World.h"
#include "ChewyMovementComponent.h"
#include <iostream>

using namespace glm;
using namespace std;

vec3 forwardDirection(std::shared_ptr<Camera> camera)
{
	return normalize(camera->cameraFront);
}

vec3 backDirection(std::shared_ptr<Camera> camera)
{
	return normalize(camera->cameraFront) * -1.0f;
}

vec3 rightDirection(std::shared_ptr<Camera> camera)
{
	return normalize(cross(camera->cameraFront, vec3(0.0, 1.0, 0.0)));
}

vec3 leftDirection(std::shared_ptr<Camera> camera)
{
	return normalize(cross(camera->cameraFront, vec3(0.0, 1.0, 0.0))) * -1.0f;
}

float angleDiff(float angle_a, float angle_b)
{
	vec2 a(cos(angle_a), sin(angle_a));
	vec2 b(cos(angle_b), sin(angle_b));

	return acos(dot(a, b));
}

void ChewyMovementComponent::update()
{
	if (camera->in_use) 
    {
		direction.x = 0.f;
		direction.y = 0.f;
		direction.z = 0.f;

		entity.last_position = entity.position;

		entity.velocity -= vec3(0, 25, 0) * seconds_passed;
		
		if (keys[GLFW_KEY_W]) {
			direction += forwardDirection(camera);
		}
		if (keys[GLFW_KEY_S]) {
			direction += backDirection(camera);
		}
		if (keys[GLFW_KEY_A]) {
			direction += leftDirection(camera);
		}
		if (keys[GLFW_KEY_D]) {
			direction += rightDirection(camera);
		}

        vec3 pos_offset;

		if (length(direction) > 0) {
            direction = normalize(direction * vec3(1, 0, 1));
                
            pos_offset = direction * CHEWY_MOVE_SPEED * seconds_passed;

			float toAngle = entity.turnAngle(direction).y;
			float fromAngle = entity.rotations.y;
			
			if (toAngle - fromAngle < -M_PI)
			{
				toAngle += 2 * M_PI;
				entity.rotations.y += (toAngle - fromAngle) * CHEWY_ROTATE_SPEED * seconds_passed;
				if (entity.rotations.y > M_PI)
					entity.rotations.y -= 2 * M_PI;
			}
			else if (toAngle - fromAngle > M_PI)
			{
				fromAngle += 2 * M_PI;
				entity.rotations.y += (toAngle - fromAngle) * CHEWY_ROTATE_SPEED * seconds_passed;
				if (entity.rotations.y < -M_PI)
					entity.rotations.y += 2 * M_PI;
			}
			else
				entity.rotations.y += (toAngle - fromAngle) * CHEWY_ROTATE_SPEED * seconds_passed;

            entity.position += pos_offset;
		}
		if (entity.position.y <= 0 && keys[GLFW_KEY_SPACE])
		{
			entity.velocity += vec3(0, 20, 0);
		}

		entity.position += entity.velocity * seconds_passed;
		if (entity.position.y < 0)
		{
			entity.position.y = 0;
			entity.velocity.y = 0;
		}
	}
}