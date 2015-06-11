#pragma once

#include "Camera.h"
#include "GameEntity.h"

#define DEBUG_SPEED 5.f

class DebugCamera : public Camera
{
public:
	DebugCamera();
	~DebugCamera();
    void mouse_update();
    void movement(GameEntity* chewy) override;
};