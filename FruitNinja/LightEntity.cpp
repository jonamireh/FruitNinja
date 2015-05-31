#include "LightEntity.h"

void LightEntity::update()
{
    if (collision_response == true)
    {
        last_position = getPosition();
        setPosition(getPosition() - glm::vec3(0.f, 10.f * seconds_passed, 0.f));
    }
}