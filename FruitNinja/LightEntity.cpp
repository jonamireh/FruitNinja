#include "LightEntity.h"
#include "main.h"

#define INTENSITY_FACTOR .1f

LightEntity::LightEntity() : light(NULL)
{
	collision_response = true;
}

LightEntity::LightEntity(glm::vec3 position, MeshSet* mesh, float intensity, MeshSet* unit_sphere, glm::vec3 color)
	: GameEntity(position, mesh), light(new Light(getPosition(), color, intensity, unit_sphere))
{
	setup_inner = true;
	bounding_box.half_width = intensity * INTENSITY_FACTOR;
	bounding_box.half_height = intensity * INTENSITY_FACTOR;
	bounding_box.half_depth = intensity * INTENSITY_FACTOR;
}

LightEntity::LightEntity(glm::vec3 position, MeshSet* mesh, float intensity, MeshSet* unit_sphere)
	: GameEntity(position, mesh), light(new Light(getPosition(), glm::vec3(1.0, 1.0, 1.0), intensity, unit_sphere))
{
	setup_inner = true;
	bounding_box.half_width = intensity * INTENSITY_FACTOR;
	bounding_box.half_height = intensity * INTENSITY_FACTOR;
	bounding_box.half_depth = intensity * INTENSITY_FACTOR;
}

void LightEntity::update()
{
	GameEntity::update();
	if (animate)
	{
		light->pos = getPosition();
		light->_transform = glm::translate(glm::mat4(1.0f), light->pos) * light->calc_scale_mat();
	}
}

void LightEntity::collision(GameEntity* entity)
{
	if (typeid(ChewyEntity) == typeid(*entity) && light != NULL)
	{
		world->set_chewy_light_distance(glm::distance(getPosition(), entity->getPosition()), glm::length(glm::vec3(bounding_box.half_width, bounding_box.half_height, bounding_box.half_depth)));
	}
}

LightEntity::~LightEntity()
{
	delete light;
}