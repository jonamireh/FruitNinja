#pragma once
#include "MeshSet.h"
#include "Material.h"
#include <memory>

class GameEntity
{

public:
    GameEntity() {}
    GameEntity(std::shared_ptr<MeshSet> mesh, Material material) : mesh(mesh), material(material) {}
	virtual void update() =0;
	std::shared_ptr<MeshSet> mesh;
	Material material;
};