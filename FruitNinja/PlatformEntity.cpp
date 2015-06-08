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
    ChewyEntity* chewy_check = dynamic_cast<ChewyEntity*>(entity);
    if (chewy_check != nullptr)
    {
        EntityBox box = bounding_box;
        box.half_depth += 0.1f;
        box.half_width += 0.1f;
        box.half_height += 0.1f;
        if (box.box_collision(entity->bounding_box))
        {
            chewy_check->setPosition(chewy_check->getPosition() + seconds_passed * movement_component.move_speed * movement_component.direction);
        }
    }
}