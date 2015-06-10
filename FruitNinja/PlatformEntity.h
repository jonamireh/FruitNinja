#pragma once

#include "World.h"
#include "GameEntity.h"
#include "PlatformMovementComponent.h"

class PlatformEntity : public GameEntity
{
    World* world;
public:
    bool pressed;
    PlatformEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points, float move_speed);
    PlatformMovementComponent movement_component;
    void update();
    void collision(GameEntity* entity) override;
};