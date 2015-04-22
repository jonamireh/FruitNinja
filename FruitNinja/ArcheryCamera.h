#pragma once

#include "Camera.h"

class ArcheryCamera : public Camera
{
public:
	ArcheryCamera();
	~ArcheryCamera();
    void mouse_update();
    void movement(double deltaTime, std::shared_ptr<GameEntity> chewy);
};