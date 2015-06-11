#include "WallOfDoomEntity.h"
#include "World.h"

WallOfDoomEntity::WallOfDoomEntity(glm::vec3 position, MeshSet* mesh, glm::vec3 direction) : GameEntity(position, mesh)
{
    this->direction = glm::normalize(direction);
    distance_traveled = 0;
    enabled = false;
}


void WallOfDoomEntity::update()
{
    if (!enabled)
        return;
    if (distance_traveled > 240.f)
        list = UNSET_DRAW(list);
    float delta_distance = 5.f * seconds_passed;
    setPosition(getPosition() + delta_distance * direction);
    distance_traveled += delta_distance;
}

void WallOfDoomEntity::collision(GameEntity* entity)
{
    if (typeid(*entity) == typeid(ChewyEntity))
    {
        // call lose stuff for chewy
    }
}