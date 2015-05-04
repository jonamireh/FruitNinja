#pragma once
#include "GameEntity.h"
#include "Camera.h"

class Skybox :
	public GameEntity
{
	std::shared_ptr<Camera>* _camera;
public:
	void update() override;
	Skybox(std::shared_ptr<Camera>* camera, std::shared_ptr<MeshSet> mesh);
	~Skybox();
};

