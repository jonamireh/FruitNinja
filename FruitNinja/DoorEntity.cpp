#include "DoorEntity.h"
#include "ChewyEntity.h"
#include "World.h"
using namespace std;

DoorEntity::DoorEntity()
{
}


DoorEntity::DoorEntity(glm::vec3 position, MeshSet* mesh, bool open, World* world) : GameEntity(position, mesh)
{
    this->open = open;
    this->world = world;
}

void DoorEntity::update()
{
}

void DoorEntity::collision(GameEntity* entity)
{
    ChewyEntity* chewy_check = dynamic_cast<ChewyEntity*>(entity);
    if (chewy_check != nullptr && open)
    {
        EntityBox box = bounding_box;
        box.half_depth += 0.5f;
        if (box.box_collision(entity->bounding_box))
            world->setup_next_courtyard();
    }
}