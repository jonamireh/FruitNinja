#pragma once

#include "World.h"
#include "GameEntity.h"
#define TIME_TO_FALL 1.0f
class FallingEntity : public GameEntity
{
    bool stepped_on;
    float elapsed_time = 0.f;
public:
    FallingEntity(glm::vec3 position, MeshSet* mesh);
    void update();
    void collision(GameEntity* entity) override;
};