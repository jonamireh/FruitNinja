#pragma once

#include "Component.h"
#include "GameEntity.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <math.h>

#define GUARD_MOVE_SPEED 10.0f
#define GUARD_ROTATE_SPEED 15.0f
#include "GuardPathingComponent.h"

class GuardMovementComponent : public Component
{
public:
	GuardMovementComponent(GameEntity& entity, std::vector<glm::vec3> control_points, float animation_time) : entity(entity), animation_time(animation_time), 
		start_position(entity.position), control_points(control_points), pathing(control_points, animation_time) {}
	void update();
	std::vector<glm::vec3> control_points;
	float animation_time;
	glm::vec3 start_position;
	float time_elapsed = 0.f;
	glm::vec3 direction;
	GuardPathingComponent pathing;

private:
	GameEntity& entity;
};