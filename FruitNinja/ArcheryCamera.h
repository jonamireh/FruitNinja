#pragma once

#include "Camera.h"
#include "MeshSet.h"
#include <memory>
class ArcheryCamera : public Camera
{
public:
	ArcheryCamera(Mesh* particle);
	ArcheryCamera();
	~ArcheryCamera();
    void mouse_update();
    void movement(std::shared_ptr<GameEntity> chewy);
	Mesh* particle;
};