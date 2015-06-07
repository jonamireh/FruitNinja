#include "ButtonEntity.h"
#include "ProjectileEntity.h"

ButtonEntity::ButtonEntity() {}

ButtonEntity::ButtonEntity(glm::vec3 position, MeshSet* mesh, string load_path, World* world) : GameEntity(position, mesh)
{
    pressed = false;
    this->load_path = load_path;
    this->world = world;
}

void ButtonEntity::update() {}

void ButtonEntity::collision(GameEntity* entity)
{
    if (!pressed)
    {
        ChewyEntity* chewy_check = dynamic_cast<ChewyEntity*>(entity);
        if (chewy_check != nullptr)
        {
            EntityBox box = bounding_box;
            box.half_height += 0.5f;
            if (box.box_collision(entity->bounding_box))
            {
                world->setup_level(load_path);
                setPosition(getPosition() - glm::vec3(0.f, bounding_box.half_height, 0.f));
                pressed = true;
                return;
            }
        }
        ProjectileEntity* arrow_check = dynamic_cast<ProjectileEntity*>(entity);
        if (arrow_check != nullptr)
        {
            EntityBox box = bounding_box;
            box.half_depth += 0.5f;
            box.half_width += 0.5f;
            if (box.box_collision(entity->bounding_box))
            {
                world->setup_level(load_path);
                //setPosition(getPosition() - glm::vec3(0.f, 0.f, 0.f));
                pressed = true;
            }
        }
    }
}