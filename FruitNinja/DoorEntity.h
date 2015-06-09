#pragma once

#include "World.h"
#include "GameEntity.h"

class DoorEntity : public GameEntity
{
public:
    bool open;
    DoorEntity();
    DoorEntity(glm::vec3 position, MeshSet* mesh, bool open);
    void update();
    void collision(GameEntity* entity) override;
};