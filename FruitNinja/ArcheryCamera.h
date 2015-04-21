#pragma once

#include "Camera.h"

class ArcheryCamera : public Camera
{
public:
	ArcheryCamera();
	~ArcheryCamera();
	virtual void cursorPosCallback(double, double);
	virtual void movement(double);
};