#pragma once

#include "Camera.h"

class ArcheryCamera : public Camera
{
public:
	ArcheryCamera();
	~ArcheryCamera();
	virtual void cursorPosCallback(double, double);
    void movement(double deltaTime, std::shared_ptr<GameEntity> chewy);
};