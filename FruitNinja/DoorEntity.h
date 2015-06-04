#pragma once

#include "World.h"
#include "GameEntity.h"

class DoorEntity : public GameEntity
{
    World* world;
public:
    bool open;
    DoorEntity();
    DoorEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, bool open, World* world);
    void update();
    void collision(std::shared_ptr<GameEntity> entity) override;
};