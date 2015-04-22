#pragma once
#include "MeshSet.h"
#include "Material.h"
#include <memory>

class GameEntity
{

public:
    GameEntity() {}
    GameEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, Material material) : position(position), mesh(mesh), material(material) {}
	virtual void update() =0;
	std::shared_ptr<MeshSet> mesh;
	Material material;
	glm::vec3 position;
};