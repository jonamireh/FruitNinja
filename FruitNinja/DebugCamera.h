#pragma once

#include "Camera.h"

class DebugCamera : Camera
{
public:
	DebugCamera();
	~DebugCamera();
	virtual void cursorPosCallback(double, double);
	virtual void keyCallback(int, int, int, int);
	virtual void movement(double);
};