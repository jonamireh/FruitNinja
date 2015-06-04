#pragma once

#include "GameEntity.h"
#include "GuardMovementComponent.h"
#include "GuardAnimationComponent.h"
#include "FrustrumCulling.h"

class GuardEntity : public GameEntity
{
public:
	GuardEntity();
	GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::vector<glm::vec3> control_points, float move_speed, bool linear_curve = false);
	GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, glm::vec3 dir);
	void update();
	void check_view(std::shared_ptr<ChewyEntity> chewy, std::vector<std::shared_ptr<GameEntity>> entities);
	glm::vec3 front;
	GuardMovementComponent move_component;
private:
	GuardAnimationComponent animComponent;
	bool static_movement = false;
};