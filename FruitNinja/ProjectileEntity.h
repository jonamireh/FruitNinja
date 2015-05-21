#pragma once

#include "GameEntity.h"
#include "ProjectileMovementComponent.h"

#define ARROW_LIFE_SPAN 5.0 //in seconds
#define ARROW_SPEED 200.0f

class ProjectileEntity : public GameEntity
{
public:
	ProjectileEntity();
	virtual ~ProjectileEntity();
	ProjectileEntity(std::shared_ptr<MeshSet> mesh, std::shared_ptr<Camera> camera);
	void update() override;
	std::shared_ptr<GameEntity> owner;
	virtual glm::mat4 getModelMat() override;
	void collision(std::shared_ptr<GameEntity> bb) override;
	bool shot;
	glm::mat4 rot;
	float timeLeft;
private:
	ProjectileMovementComponent movement;
};