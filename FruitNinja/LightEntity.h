#pragma once

#include "GameEntity.h"
#include "Light.h"
#include "AudioManager.h"
#include "World.h"

class LightEntity : public GameEntity
{
public:
	LightEntity();
	LightEntity(glm::vec3 position, MeshSet* mesh, float intensity, MeshSet* unit_sphere);
	LightEntity(glm::vec3 position, MeshSet* mesh, float intensity, MeshSet* unit_sphere, glm::vec3 color);
	~LightEntity();
	void update();
	Light* light;
	void collision(GameEntity* entity);
};