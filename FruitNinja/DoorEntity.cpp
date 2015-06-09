#include "DoorEntity.h"
#include "ChewyEntity.h"
#include "World.h"
#include "main.h"
using namespace std;

DoorEntity::DoorEntity()
{
}


DoorEntity::DoorEntity(glm::vec3 position, MeshSet* mesh, bool open) : GameEntity(position, mesh)
{
    this->open = open;
}

void DoorEntity::update()
{
}

void DoorEntity::collision(GameEntity* entity)
{
    ChewyEntity* chewy_check = dynamic_cast<ChewyEntity*>(entity);
    if (chewy_check != nullptr && open)
    {
        world->setup_next_courtyard();
    }
}