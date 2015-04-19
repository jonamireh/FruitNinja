#pragma once
#include "MeshSet.h"
#include "Material.h"

class GameEntity
{

public:
	GameEntity(MeshSet& mesh, Material material);
	virtual void update() =0;
	MeshSet& mesh;
	Material material;
};