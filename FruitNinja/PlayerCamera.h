#pragma once

#include "Camera.h"

class PlayerCamera : Camera
{
	static const float MinVerticalAngle;
	vec3 lookAtPoint;
public:
	PlayerCamera();
	PlayerCamera(vec3 lookAt);
	~PlayerCamera();
	virtual void cursorPosCallback(double, double);
	virtual void keyCallback(int, int, int, int);
	virtual void movement(double);
	mat4 getViewMatrix();
};