#include "ButtonEntity.h"
#include "ProjectileEntity.h"
#include "AudioManager.h"
#include "main.h"
#include "ObstacleEntity.h"
#include "GateEntity.h"

ButtonEntity::ButtonEntity() {}

ButtonEntity::ButtonEntity(glm::vec3 position, MeshSet* mesh, vector<string> on_press_levels, 
    vector<string> on_press_platforms, vector<string> other_button_files, vector<string> on_press_cinematic_file, bool gate_open) : GameEntity(position, mesh)
{
    pressed = false;
    this->on_press_levels = on_press_levels;
    this->on_press_platforms = on_press_platforms;
    this->other_button_files = other_button_files;
    this->on_press_cinematic_file = on_press_cinematic_file;
    this->gate_open = gate_open;

    if (mesh == world->meshes["button"])
    {
        world->entities.push_back(new ObstacleEntity(position, world->meshes.at("button_base")));
        world->entities.back()->list = UNSET_OCTTREE((world->entities.back()->list));
    }
}

void ButtonEntity::update() {
	GameEntity::update();
}

void ButtonEntity::button_pressed()
{
    // loads all the level files in
    for (int i = 0; i < on_press_levels.size(); i++)
        world->setup_level(on_press_levels.at(i), true);
    // loads in the moving platforms
    for (int i = 0; i < on_press_platforms.size(); i++)
        world->setup_moving_platform(on_press_platforms.at(i));
    // loads in the other button files
    for (int i = 0; i < other_button_files.size(); i++)
        world->load_button(other_button_files.at(i));
    // play cinematic camera if has one
    for (int i = 0; i < on_press_cinematic_file.size(); i++)
        world->setup_cinematic_camera(on_press_cinematic_file.at(i), true);

    if (gate_open)
    {
        for (int i = 0; i < world->entities.size(); i++)
        {
            if (typeid(GateEntity) == typeid(*world->entities.at(i)))
                world->entities.at(i)->list = UNSET_DRAW(world->entities.at(i)->list);
        }
    }
}

void ButtonEntity::collision(GameEntity* entity)
{
    if (!pressed)
    {
        if (typeid(ChewyEntity) == typeid(*entity) && mesh == world->meshes["button"])
        {
            button_pressed();
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
            button_pressed();
            pressed = true;
            playPressedSound();
        }
    }
}

void ButtonEntity::playPressedSound()
{
	AudioManager::instance()->play3D(assetPath + "button.wav", getPosition(), 10.0f, false);
}