#pragma once

#include "World.h"
#include "GameEntity.h"

class FallingEntity : public GameEntity
{
public:
    bool pressed;
    FallingEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points, float move_speed);
    void update();
    void collision(GameEntity* entity) override;
};