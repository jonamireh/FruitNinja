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
    bool _falling;
	ChewyMovementComponent moveComponent;
	ChewyAnimationComponent animComponent;
	bool isCaught;
	glm::mat4 getModelMat();
	std::vector<std::vector<glm::mat4>>* getBoneTransformations() override;
};