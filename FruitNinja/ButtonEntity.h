#pragma once

#include "World.h"
#include "GameEntity.h"

class ButtonEntity : public GameEntity
{
    World* world;
    string load_path;

	void playPressedSound();
public:
    bool pressed;
    ButtonEntity();
    ButtonEntity(glm::vec3 position, MeshSet* mesh, string load_path, World* world);
    void update();
    void collision(GameEntity* entity) override;
};