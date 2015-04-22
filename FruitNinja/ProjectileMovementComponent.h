#pragma once

#include "Component.h"
#include "Camera.h"
//#include <memory>
//#include "ProjectileEntity.h"

#define ARROW_SPEED 30.0f

class ProjectileEntity;

class ProjectileMovementComponent : public Component
{
public:
	ProjectileMovementComponent(ProjectileEntity& entity, std::shared_ptr<Camera> camera) : entity(entity), camera(camera) {}
	void update();

private:
	ProjectileEntity& entity;
	std::shared_ptr<Camera> camera;
};