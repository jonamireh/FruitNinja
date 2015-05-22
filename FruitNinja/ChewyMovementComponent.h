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
	ChewyMovementComponent(GameEntity& entity, std::shared_ptr<Camera> player_cam, std::shared_ptr<Camera> archery_cam) : entity(entity), player_cam(player_cam), archery_cam(archery_cam) {}
	void update();
	glm::vec3 direction;

private:
	GameEntity& entity;
	std::shared_ptr<Camera> player_cam;
	std::shared_ptr<Camera> archery_cam;
};