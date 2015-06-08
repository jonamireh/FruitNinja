#include "FallingEntity.h"
#include "SpikeEntity.h"
#include "World.h"
#include <glm/gtx/string_cast.hpp>


FallingEntity::FallingEntity(glm::vec3 position, MeshSet* mesh) :
GameEntity(position, mesh, true)
{
    velocity = glm::vec3(0.f);
    stepped_on = false;
}

void FallingEntity::update()
{
    if (stepped_on)
    {
        setPosition(getPosition() + velocity * seconds_passed);
        elapsed_time += seconds_passed;
        if (elapsed_time > TIME_TO_FALL)
            velocity.y -= GRAVITY * 0.75f * seconds_passed;
        //else
            // have the box shake to show it will fall soon
    }
}

void FallingEntity::collision(GameEntity* entity)
{
    ChewyEntity* chewy_check = dynamic_cast<ChewyEntity*>(entity);
    if (chewy_check != nullptr)
    {
        EntityBox box = bounding_box;
        box.half_height += 0.2f;
        if (box.box_collision(entity->bounding_box))
            stepped_on = true;
    }
    SpikeEntity* spike_entity = dynamic_cast<SpikeEntity*>(entity);
    if (spike_entity != nullptr)
    {
        EntityBox box = bounding_box;
        box.half_height += 0.1f;
        if (box.box_collision(entity->bounding_box))
            stepped_on = false; // instead of this remove this object
    }

}