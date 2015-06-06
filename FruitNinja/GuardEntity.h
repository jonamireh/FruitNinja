#pragma once

#include "GameEntity.h"
#include "GuardMovementComponent.h"
#include "GuardAnimationComponent.h"
#include "FrustrumCulling.h"

class GuardEntity : public GameEntity
{
public:
	GuardEntity();
	GuardEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points, float move_speed, bool linear_curve = false);
	GuardEntity(glm::vec3 position, MeshSet* mesh, glm::vec3 dir);
	void update();
	bool check_view(ChewyEntity* chewy, std::vector<GameEntity*> entities);
	glm::vec3 front;
	GuardMovementComponent move_component;
private:
	GuardAnimationComponent animComponent;
	bool static_movement = false;
};