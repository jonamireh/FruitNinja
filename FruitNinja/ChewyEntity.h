#pragma once

#include "GameEntity.h"
#include "ChewyMovementComponent.h"

class ChewyEntity : public GameEntity
{
public:
	ChewyEntity();
	ChewyEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::shared_ptr<Camera> camera);
	void update();
	void collision(std::shared_ptr<BoundingBox> bb);
	float up_velocity = 0;
private:
	ChewyMovementComponent moveComponent;
};