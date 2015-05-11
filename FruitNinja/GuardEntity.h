#pragma once

#include "GameEntity.h"
#include "GuardMovementComponent.h"

class GuardEntity : public GameEntity
{
public:
	GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, float time, glm::vec3 direction);
	void update();
	GuardMovementComponent move_component;
	void collisionBS(std::shared_ptr<GameEntity> entity);
	float getRadius();
private:
};