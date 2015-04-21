#pragma once

#include "Camera.h"
#include "GameEntity.h"

class DebugCamera : public Camera
{
public:
	DebugCamera();
	~DebugCamera();
	void cursorPosCallback(double, double);
    void movement(double deltaTime, std::shared_ptr<GameEntity> chewy);
};