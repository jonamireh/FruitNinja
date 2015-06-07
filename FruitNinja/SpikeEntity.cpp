#include "SpikeEntity.h"

SpikeEntity::SpikeEntity() {}

SpikeEntity::SpikeEntity(glm::vec3 position, MeshSet* mesh, World* world) : GameEntity(position, mesh)
{
    this->world = world;
}

void SpikeEntity::update() {}

void SpikeEntity::collision(GameEntity* entity)
{
    ChewyEntity* chewy_check = dynamic_cast<ChewyEntity*>(entity);
    if (chewy_check != nullptr)
    {
        EntityBox box = bounding_box;
        box.half_height += 0.35f;
        if (box.box_collision(entity->bounding_box))
        {
            world->lose_condition();
        }
    }
}