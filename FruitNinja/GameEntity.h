#pragma once
#include "MeshSet.h"
#include "Material.h"
#include <memory>

class GameEntity
{

public:
    GameEntity() {}
    GameEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : position(position), mesh(mesh) {}
	virtual void update() =0;
	std::shared_ptr<MeshSet> mesh;
	glm::vec3 position;
};