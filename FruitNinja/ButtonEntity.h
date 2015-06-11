#pragma once

#include "GameEntity.h"

class ButtonEntity : public GameEntity
{
    std::vector<std::string> on_press_levels;
    std::vector<std::string> on_press_platforms;
    std::vector<std::string> other_button_files;
    std::vector<std::string> on_press_cinematic_file;
    bool gate_open;
    bool wall_of_doom;
	void playPressedSound();
public:
    bool pressed;
    ButtonEntity();
    ButtonEntity(glm::vec3 position, MeshSet* mesh, std::vector<std::string> on_press_levels,
        std::vector<std::string> on_press_platforms, std::vector<std::string> other_button_files, std::vector<std::string> on_press_cinematic_file, bool gate_open, bool wall_of_doom);
    void update();
    void button_pressed();
    void collision(GameEntity* entity) override;
};