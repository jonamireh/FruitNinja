#pragma once

#include "World.h"
#include "GameEntity.h"

class ButtonEntity : public GameEntity
{
    World* world;
    vector<string> on_press_levels;
    vector<string> on_press_platforms;
    vector<string> other_button_files;

	void playPressedSound();
public:
    bool pressed;
    ButtonEntity();
    ButtonEntity(glm::vec3 position, MeshSet* mesh, vector<string> on_press_levels, vector<string> on_press_platforms, vector<string> other_button_files, World* world);
    void update();
    void button_pressed();
    void collision(GameEntity* entity) override;
};