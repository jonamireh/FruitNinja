#pragma once
#include "MeshSet.h"
#include "Material.h"
#include <memory>

class GameEntity
{

public:
    GameEntity() {}
    GameEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : position(position), mesh(mesh), rotations(glm::vec3(0.f, 0.f, 0.f)) {}
	virtual void update() =0;
	std::shared_ptr<MeshSet> mesh;
	glm::vec3 position;
    glm::vec3 rotations;
	void turnTo(glm::vec3);
};