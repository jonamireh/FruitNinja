#pragma once

#include "GameEntity.h"

class ObstacleEntity : public GameEntity
{
public:
	ObstacleEntity();
	ObstacleEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh);
	void update();
private:
};