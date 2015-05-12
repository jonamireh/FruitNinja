#pragma once

#include "GameEntity.h"
#include "GuardMovementComponent.h"

class GuardEntity : public GameEntity
{
public:
	GuardEntity();
	GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::vector<glm::vec3> control_points, float animation_time);
	void update();
	void collision(std::shared_ptr<GameEntity> entity);
	float getRadius();
	glm::vec3 front;
	GuardMovementComponent move_component;
private:
};