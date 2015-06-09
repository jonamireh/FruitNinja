#include "RisingMovementComponent.h"
#include "World.h"

RisingMovementComponent::RisingMovementComponent()
{

}

RisingMovementComponent::RisingMovementComponent(GameEntity* entity)
{
    entity_to_move = entity;
    final_position = entity->getPosition();
    current_position = entity->getPosition();
}

void RisingMovementComponent::update()
{
    if (current_courtyard != 3)
        return;

    if (!initialized)
    {
        initialized = true;
        current_position = entity_to_move->getPosition() - glm::vec3(0.f, 50.f, 0.f);
        entity_to_move->setPosition(current_position);
    }
    if (!completed)
    {
        if (entity_to_move->getPosition().y >= final_position.y)
        {
            entity_to_move->setPosition(final_position);
            completed = true;
        }
        else
        {
            current_position += glm::vec3(0.f, 5.f * seconds_passed, 0.f);
            entity_to_move->setPosition(current_position);
        }
    }
}