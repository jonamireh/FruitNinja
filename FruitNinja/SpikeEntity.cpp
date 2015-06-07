#include "SpikeEntity.h"

SpikeEntity::SpikeEntity() {}

SpikeEntity::SpikeEntity(glm::vec3 position, MeshSet* mesh, World* world) : GameEntity(position, mesh)
{
    this->world = world;
}

void SpikeEntity::update() {}

void SpikeEntity::collision(GameEntity* entity)
{
}