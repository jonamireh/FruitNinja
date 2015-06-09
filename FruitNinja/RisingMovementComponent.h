#pragma once

#include "Component.h"
#include "GameEntity.h"
#include <glm/glm.hpp>


class RisingMovementComponent : public Component
{
    bool initialized = false;
    bool completed = false;
    glm::vec3 current_position;
    glm::vec3 final_position;
    GameEntity* entity_to_move;
public:
    RisingMovementComponent();
    RisingMovementComponent(GameEntity* entity);
    void update();
    glm::vec3 direction;
};