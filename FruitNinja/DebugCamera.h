#pragma once

#include "Camera.h"

class DebugCamera : public Camera
{
public:
	DebugCamera();
	~DebugCamera();
	void cursorPosCallback(double, double);
	void keyCallback(int, int, int, int);
	void movement(double);
};