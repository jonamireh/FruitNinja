#pragma once

#include "GameEntity.h"
#include "GuardMovementComponent.h"

class GuardEntity : public GameEntity
{
public:
	GuardEntity();
	GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, float time, glm::vec3 direction);
	void update();
	GuardMovementComponent move_component;
private:
};