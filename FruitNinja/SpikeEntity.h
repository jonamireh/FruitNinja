#pragma once

#include "World.h"
#include "GameEntity.h"

class SpikeEntity : public GameEntity
{
public:
    World* world;
    bool pressed;
    SpikeEntity();
    SpikeEntity(glm::vec3 position, MeshSet* mesh, World* world);
    void update();
    void collision(GameEntity* entity) override;
};