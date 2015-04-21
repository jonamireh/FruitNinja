#pragma once

#include "Camera.h"

class DebugCamera : public Camera
{
public:
	DebugCamera();
	~DebugCamera();
	void cursorPosCallback(double, double);
	void movement(double);
};