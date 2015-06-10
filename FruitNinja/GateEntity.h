#pragma once
#include "GameEntity.h"

class GateEntity : public GameEntity
{
public:
    GateEntity(glm::vec3 position, MeshSet* mesh);
    void update();
    void collision(GameEntity* entity) override;
};