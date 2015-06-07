#pragma once

#include "GameEntity.h"
#include "ChewyMovementComponent.h"
#include "BasicAnimationComponent.h"
#include "ChewyAnimationComponent.h"

class ChewyEntity : public GameEntity
{
public:
	ChewyEntity();
	ChewyEntity(glm::vec3 position, MeshSet* mesh, Camera* player_cam, Camera* archery_cam);
	void update();
	void collision(GameEntity* entity) override;
	void set_material(Material material);
    bool _falling;
	ChewyMovementComponent moveComponent;
	ChewyAnimationComponent animComponent;
	bool isCaught;

	std::vector<std::vector<glm::mat4>>* getBoneTrans() override;
};