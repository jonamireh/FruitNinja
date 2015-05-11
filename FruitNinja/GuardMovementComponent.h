#pragma once

#include "Component.h"
#include "GameEntity.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <math.h>

#define GUARD_MOVE_SPEED 20.0f
#define GUARD_ROTATE_SPEED 15.0f

class GuardMovementComponent : public Component
{
public:
	GuardMovementComponent(GameEntity& entity, float time, glm::vec3 direction) : entity(entity), time(time), start_position(entity.position), direction(direction) {}
	void update();
	glm::vec3 direction;
	float time;
	glm::vec3 start_position;
	float time_elapsed = 0.f;

private:
	GameEntity& entity;
};