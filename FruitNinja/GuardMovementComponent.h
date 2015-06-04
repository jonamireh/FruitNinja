#pragma once

#include "Component.h"
#include "GameEntity.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <math.h>

#define GUARD_ROTATE_SPEED 15.0f
#include "GuardPathingComponent.h"

class GuardMovementComponent : public Component
{
public:
	GuardMovementComponent(GameEntity& entity, std::vector<glm::vec3> control_points, float move_speed, bool linear_curve) : entity(entity),
		start_position(entity.getPosition()), control_points(control_points), move_speed(move_speed), pathing(control_points, move_speed, linear_curve) {}
	GuardMovementComponent(GameEntity& entity, glm::vec3 dir) : entity(entity), direction(dir) {}
	void update(bool static_movement);
	void update() {}
	std::vector<glm::vec3> control_points;
	glm::vec3 start_position;
	glm::vec3 direction;
	float move_speed;
	GuardPathingComponent pathing;

private:
	GameEntity& entity;
};