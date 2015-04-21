#pragma once

#include "Camera.h"
#include "GameEntity.h"

class PlayerCamera : public Camera
{
	static const float MinVerticalAngle;
    glm::vec3 lookAtPoint;
public:
	PlayerCamera();
	PlayerCamera(glm::vec3 lookAt);
	~PlayerCamera();
	virtual void cursorPosCallback(double, double);
    void movement(double deltaTime, std::shared_ptr<GameEntity> chewy);
    glm::mat4 getViewMatrix();
};