#pragma once
#include "GameEntity.h"
#include "Camera.h"

class Skybox : public GameEntity
{
	Camera** _camera;
public:
	void update() override;
	Skybox(Camera** camera, MeshSet* mesh);
	~Skybox();
};

