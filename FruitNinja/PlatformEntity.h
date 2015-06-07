#pragma once

#include "World.h"
#include "GameEntity.h"
#include "GuardMovementComponent.h"

class PlatformEntity : public GameEntity
{
    World* world;
public:
    bool pressed;
    PlatformEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points, float move_speed);
    GuardMovementComponent movement_component;
    void update();
    void collision(GameEntity* entity) override;
};