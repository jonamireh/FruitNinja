#pragma once

#include "GameEntity.h"

class ObstacleEntity : public GameEntity
{
public:
    ObstacleEntity();
    ObstacleEntity(glm::vec3 position, MeshSet* mesh);
    void update();
    void collision(GameEntity* entity) override;
};