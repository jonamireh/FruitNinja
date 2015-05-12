#pragma once

#include "GameEntity.h"
#include "ChewyMovementComponent.h"
#include "BasicAnimationComponent.h"

class ChewyEntity : public GameEntity
{
public:
	ChewyEntity();
	ChewyEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::shared_ptr<Camera> camera);
	void update();
	void collision(std::shared_ptr<GameEntity> entity);
	void set_material(Material material);
	float up_velocity = 0;
    bool _falling;

private:
	ChewyMovementComponent moveComponent;
	BasicAnimationComponent animComponent;
};