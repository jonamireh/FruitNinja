#pragma once

#include "GameEntity.h"
#include "ChewyMovementComponent.h"
#include "ChewyAnimationComponent.h"

class ChewyEntity : public GameEntity
{
public:
	//ChewyEntity();
	ChewyEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::shared_ptr<Camera> camera);
	void update();
	void collisionBB(std::shared_ptr<GameEntity> entity);
	float up_velocity = 0;
	aiAnimation *current_animation;
	void set_material(Material mat);
private:
	ChewyMovementComponent moveComponent;
	ChewyAnimationComponent animComponent;
};