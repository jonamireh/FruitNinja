#pragma once

#include "GameEntity.h"

#define BOUNCE_HEIGHT 1.0
#define BOUNCE_SPEED 2.0
#define SPIN_SPEED 2.0 

class CollectableEntity : public GameEntity
{
public:
	CollectableEntity();
	virtual ~CollectableEntity();
	CollectableEntity(glm::vec3 position, MeshSet* mesh, int numArrows = 1);
	void update() override;
	void collision(GameEntity* entity) override;
private:
	glm::vec3 center;
	double total_secs = 0.0;
	int number;
};