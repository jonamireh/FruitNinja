#include "DoorEntity.h"
#include "ChewyEntity.h"
#include "World.h"
using namespace std;

DoorEntity::DoorEntity()
{
}


DoorEntity::DoorEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, bool open, World* world) : GameEntity(position, mesh)
{
    this->open = open;
    this->world = world;
}

void DoorEntity::update()
{
}

void DoorEntity::collision(std::shared_ptr<GameEntity> entity)
{
    shared_ptr<ChewyEntity> chewy_check = dynamic_pointer_cast<ChewyEntity>(entity);
    if (chewy_check != nullptr && open)
    {
        world->setup_next_courtyard();
    }
}