#pragma once

#include "GameEntity.h"

class WallOfDoomEntity : public GameEntity
{
    glm::vec3 direction;
    float distance_traveled;
public:
    bool enabled;
    WallOfDoomEntity(glm::vec3 position, MeshSet* mesh, glm::vec3 direction);
    void update();
    void collision(GameEntity* entity) override;
};