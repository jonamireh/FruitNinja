#pragma once

#include "GameEntity.h"
#include "ProjectileMovementComponent.h"

#define ARROW_LIFE_SPAN 5.0 //in seconds, no longer 
#define ARROW_SPEED 100.0f
#define SHOOT_TIME_SPEED 1.0

class ProjectileEntity : public GameEntity
{
public:
	ProjectileEntity();
	virtual ~ProjectileEntity();
	ProjectileEntity(MeshSet* mesh, Camera* camera);
	void update() override;
	GameEntity* owner;
	virtual glm::mat4 getModelMat() override;
	void collision(GameEntity* bb) override;
	bool shot;
	glm::mat4 rot;
	float timeLeft;
	float dist;
	glm::vec3 getVelocity();
private:
	ProjectileMovementComponent movement;
};