#pragma once

#include "Camera.h"
#include "GameEntity.h"
#include <glm/gtx/rotate_vector.hpp>

class PlayerCamera : public Camera
{
	static const float MinVerticalAngle;
    glm::vec3 lookAtPoint;
    float radius;
public:
	PlayerCamera();
	PlayerCamera(glm::vec3 lookAt);
	~PlayerCamera();
    void mouse_update();
    void movement(double deltaTime, std::shared_ptr<GameEntity> chewy);
    glm::mat4 getViewMatrix();
};