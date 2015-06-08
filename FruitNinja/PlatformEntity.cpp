#include "PlatformEntity.h"
#include <glm/gtx/string_cast.hpp>


PlatformEntity::PlatformEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points, float move_speed) : 
GameEntity(position, mesh, true), movement_component(*this, control_points, move_speed, true)
{

}

void PlatformEntity::update()
{
    movement_component.update(false);
}

void PlatformEntity::collision(GameEntity* entity)
{
    if (typeid(ChewyEntity) == typeid(*entity))
    {
        entity->setPosition(entity->getPosition() + seconds_passed * movement_component.move_speed * movement_component.direction);
    }
}