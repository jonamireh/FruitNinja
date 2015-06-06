#pragma once

#include "World.h"
#include "GameEntity.h"

class DoorEntity : public GameEntity
{
    World* world;
public:
    bool open;
    DoorEntity();
    DoorEntity(glm::vec3 position, MeshSet* mesh, bool open, World* world);
    void update();
    void collision(GameEntity* entity) override;
};