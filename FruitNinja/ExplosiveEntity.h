#pragma once

#include "GameEntity.h"

class ExplosiveEntity : public GameEntity
{
public:
	ExplosiveEntity();
	ExplosiveEntity(glm::vec3 position, MeshSet* mesh, float explosion_radius);
	virtual ~ExplosiveEntity();
	void update() override;
	void collision(GameEntity* entity) override;
private:
	bool exploding;
	float expScale;
	double ttl;
	double explosion_length = 0.5;
};