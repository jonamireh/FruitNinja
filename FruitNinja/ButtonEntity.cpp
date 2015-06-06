#include "ButtonEntity.h"

ButtonEntity::ButtonEntity() {}

ButtonEntity::ButtonEntity(glm::vec3 position, MeshSet* mesh, World* world) : GameEntity(position, mesh)
{
    pressed = false;
    this->world = world;
}

void ButtonEntity::update() {}

void ButtonEntity::collision(GameEntity* entity)
{
    ChewyEntity* chewy_check = dynamic_cast<ChewyEntity*>(entity);
    if (chewy_check != nullptr && !pressed)
    {
        EntityBox box = bounding_box;
        box.half_height += 0.5f;
        if (box.box_collision(entity->bounding_box))
        {
            world->setup_level(assetPath + "third_courtyard_button_one.txt");
            setPosition(getPosition() - glm::vec3(0.f, bounding_box.half_height, 0.f));
            pressed = true;
        }
    }
}