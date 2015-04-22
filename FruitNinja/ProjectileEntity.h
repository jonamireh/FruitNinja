#pragma once

#include "GameEntity.h"
#include "ProjectileMovementComponent.h"

class ProjectileEntity : public GameEntity
{
public:
	ProjectileEntity();
	ProjectileEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh,
		std::shared_ptr<GameEntity> owner, std::shared_ptr<Camera> camera);
	void update();
	std::shared_ptr<GameEntity> owner;
private:
	ProjectileMovementComponent movement;
};