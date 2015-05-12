#pragma once

#include "GameEntity.h"
#include "ProjectileMovementComponent.h"

class ProjectileEntity : public GameEntity
{
public:
	ProjectileEntity();
	virtual ~ProjectileEntity();
	ProjectileEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh,
		std::shared_ptr<GameEntity> owner, std::shared_ptr<Camera> camera);
	void update() override;
	std::shared_ptr<GameEntity> owner;
	virtual glm::mat4 getModelMat() override;
	void collision(std::shared_ptr<GameEntity> bb) override;
	bool shot;
	glm::mat4 rot;
private:
	ProjectileMovementComponent movement;
};