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
	CollectableEntity(glm::vec3 position, MeshSet* mesh, bool animate = true, int numArrows = 1);
	void update() override;
	void collision(GameEntity* entity) override;
	void custom_rotate(glm::mat4& rot);
	glm::mat4 getModelMat();
private:
	glm::vec3 center;
	double total_secs = 0.0;
	int number;
	bool animate;
	glm::mat4 rot;
};