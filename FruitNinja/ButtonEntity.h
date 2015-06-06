#pragma once

#include "World.h"
#include "GameEntity.h"

class ButtonEntity : public GameEntity
{
    World* world;
public:
    bool pressed;
    ButtonEntity();
    ButtonEntity(glm::vec3 position, MeshSet* mesh, World* world);
    void update();
    void collision(GameEntity* entity) override;
};