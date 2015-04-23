#pragma once

#include "Component.h"
#include "GameEntity.h"
#include "Camera.h"
#include <math.h>

#define CHEWY_MOVE_SPEED 15.0f
#define CHEWY_ROTATE_SPEED ((float)(M_PI * 2))

class ChewyMovementComponent : public Component
{
public:
	ChewyMovementComponent(GameEntity& entity, std::shared_ptr<Camera> camera) : entity(entity), camera(camera) {}
	void update();

private:
	GameEntity& entity;
	std::shared_ptr<Camera> camera;
};