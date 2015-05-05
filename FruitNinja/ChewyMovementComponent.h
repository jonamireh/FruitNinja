#pragma once

#include "Component.h"
#include "GameEntity.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <math.h>

#define CHEWY_MOVE_SPEED 20.0f
#define CHEWY_ROTATE_SPEED 15.0f

class ChewyMovementComponent : public Component
{
public:
	ChewyMovementComponent(GameEntity& entity, std::shared_ptr<Camera> camera) : entity(entity), camera(camera) {}
	void update();
	glm::vec3 direction;

private:
	GameEntity& entity;
	std::shared_ptr<Camera> camera;
};