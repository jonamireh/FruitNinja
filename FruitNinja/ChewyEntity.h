#pragma once

#include "GameEntity.h"
#include "ChewyMovementComponent.h"
#include "BasicAnimationComponent.h"
#include "ChewyAnimationComponent.h"

class ChewyEntity : public GameEntity
{
public:
	ChewyEntity();
	ChewyEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::shared_ptr<Camera> player_cam, std::shared_ptr<Camera> archery_cam);
	void update();
	void collision(std::shared_ptr<GameEntity> entity);
	void set_material(Material material);
    bool _falling;

private:
	ChewyMovementComponent moveComponent;
	ChewyAnimationComponent animComponent;
};