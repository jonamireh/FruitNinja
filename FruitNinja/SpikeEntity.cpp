#include "SpikeEntity.h"
#include "ChewyEntity.h"

SpikeEntity::SpikeEntity() {}

SpikeEntity::SpikeEntity(glm::vec3 position, MeshSet* mesh, World* world) : GameEntity(position, mesh)
{
    this->world = world;
}

void SpikeEntity::update() {
	GameEntity::update();
}

void SpikeEntity::collision(GameEntity* entity)
{
	if (typeid(ChewyEntity) == typeid(*entity)) {
		if (world->getState() == HIDDEN)
			world->delayed_lose_condition();
	}
}