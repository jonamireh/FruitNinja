#pragma once

#include "Camera.h"
#include "GameEntity.h"

class DebugCamera : public Camera
{
public:
	DebugCamera();
	~DebugCamera();
    void mouse_update();
    void movement(double deltaTime, std::shared_ptr<GameEntity> chewy);
};