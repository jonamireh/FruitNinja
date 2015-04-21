#pragma once

#include "Camera.h"

class PlayerCamera : public Camera
{
	static const float MinVerticalAngle;
    glm::vec3 lookAtPoint;
public:
	PlayerCamera();
	PlayerCamera(glm::vec3 lookAt);
	~PlayerCamera();
	virtual void cursorPosCallback(double, double);
	virtual void movement(double);
    glm::mat4 getViewMatrix();
};