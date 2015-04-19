#pragma once

#include "Camera.h"

class ArcheryCamera : public Camera
{
public:
	ArcheryCamera();
	~ArcheryCamera();
	virtual void cursorPosCallback(double, double);
	virtual void keyCallback(int, int, int, int);
	virtual void movement(double);
};