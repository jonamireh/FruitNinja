#pragma once

#include "Component.h"
#include "Camera.h"
//#include <memory>
//#include "ProjectileEntity.h"

class ProjectileEntity;

class ProjectileMovementComponent : public Component
{
public:
	ProjectileMovementComponent(ProjectileEntity& entity, Camera* camera) : entity(entity), camera(camera), velocity(glm::vec3(0.0f, 0.0f, 0.0f)) {}
	void update();
	glm::vec3 velocity;
private:
	ProjectileEntity& entity;
	Camera* camera;
	
};