#pragma once

#include "GameEntity.h"
#include "GuardMovementComponent.h"
#include "FrustrumCulling.h"

class GuardEntity : public GameEntity
{
public:
	GuardEntity();
	GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::vector<glm::vec3> control_points, float move_speed, bool linear_curve = false);
	void update();
	void collision(std::shared_ptr<GameEntity> entity);
	float getRadius();
	void check_view(std::shared_ptr<ChewyEntity> chewy, std::vector<std::shared_ptr<GameEntity>> entities);
	glm::vec3 front;
	GuardMovementComponent move_component;
private:
};