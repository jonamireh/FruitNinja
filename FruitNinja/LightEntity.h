#pragma once

#include "GameEntity.h"
#include "Light.h"
#include "AudioManager.h"

class LightEntity : public GameEntity
{
public:
    LightEntity() : light(NULL)
	{
        collision_response = true;
		AudioManager::instance()->play3DLoop("assets/flame.wav", light->pos, true);
	}
	//color will default to white, probably use this
	LightEntity(glm::vec3 position, MeshSet* mesh, float intensity, MeshSet* unit_sphere)
		: GameEntity(position, mesh), light(new Light(getPosition(), glm::vec3(1.0, 1.0, 1.0), intensity, unit_sphere))
	{
		AudioManager::instance()->play3DLoop("assets/flame.wav", light->pos, true);
	}

	LightEntity(glm::vec3 position, MeshSet* mesh, float intensity, MeshSet* unit_sphere, glm::vec3 color)
		: GameEntity(position, mesh), light(new Light(getPosition(), color, intensity, unit_sphere))
	{
		AudioManager::instance()->play3DLoop("assets/flame.wav", light->pos, true);
	}
	void update();
	Light* light;
};