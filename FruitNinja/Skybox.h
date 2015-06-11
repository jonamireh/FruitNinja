#pragma once
#include "GameEntity.h"
#include "Camera.h"

class Skybox : public GameEntity
{
public:
	Camera** _camera;
	void update() override;
	Skybox(Camera** camera, MeshSet* mesh);
	~Skybox();
};

