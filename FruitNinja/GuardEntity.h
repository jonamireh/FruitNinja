#pragma once

#include "GameEntity.h"
#include "GuardMovementComponent.h"
#include "GuardAnimationComponent.h"
#include "FrustrumCulling.h"

class GuardEntity : public GameEntity
{
public:
	GuardEntity();
	GuardEntity(MeshSet* mesh, GAnimationState anim_st);
	~GuardEntity();
	//GuardEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points, float move_speed, bool linear_curve = false);
	//GuardEntity(glm::vec3 position, MeshSet* mesh, glm::vec3 dir);

	GuardEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points, float move_speed, GuardAnimationComponent* animComp, bool linear_curve = false);
	GuardEntity(glm::vec3 position, MeshSet* mesh, glm::vec3 dir, GuardAnimationComponent* animComp);


	void update();
	bool check_view(ChewyEntity* chewy, std::vector<GameEntity*> entities);
	glm::vec3 front;
	GuardMovementComponent move_component;
	std::vector<std::vector<glm::mat4>>* getBoneTrans() override;
	GuardAnimationComponent* animComponent;
	bool animCompOwner;
private:
	bool static_movement = false;
};