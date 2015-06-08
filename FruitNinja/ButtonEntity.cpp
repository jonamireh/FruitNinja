#include "ButtonEntity.h"
#include "ProjectileEntity.h"
#include "AudioManager.h"
#include "World.h"

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
        if (typeid(ChewyEntity) == typeid(*entity))
        {
            world->setup_level(load_path);
            pressed = true;
            setPosition(getPosition() - glm::vec3(0.f, 2.f * bounding_box.half_height, 0.f));
            playPressedSound();
            glm::vec3 dir_to_move = glm::vec3(getPosition().x, 0.f, getPosition().z) - glm::vec3(entity->getPosition().x, 0.f, entity->getPosition().z);
            dir_to_move = glm::normalize(dir_to_move);
            entity->setPosition(entity->getPosition() + glm::vec3(0.f, bounding_box.half_height, 0.f) + 0.5f * dir_to_move);
            return;
        }
        ProjectileEntity* arrow_check = dynamic_cast<ProjectileEntity*>(entity);
        if (arrow_check != nullptr)
        {
            world->setup_level(load_path);
            pressed = true;
            playPressedSound();
        }
    }
}

void ButtonEntity::playPressedSound()
{
	AudioManager::instance()->play3D(assetPath + "button.wav", getPosition(), 10.0f, false);
}