#pragma once

#include "Component.h"
#include "GameEntity.h"
#include "Camera.h"

#define CHEWY_MOVE_SPEED 5.0f

class ChewyMovementComponent : public Component
{
public:
	ChewyMovementComponent(std::shared_ptr<GameEntity> entity, std::shared_ptr<Camera> camera) : entity(entity), camera(camera) {}
	void update();

private:
	std::shared_ptr<GameEntity> entity;
	std::shared_ptr<Camera> camera;
};