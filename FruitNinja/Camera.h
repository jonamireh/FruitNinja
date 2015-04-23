#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "GameEntity.h"

class ChewyEntity;
static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

/*
// Where this is called from use this code
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
glfwSetCursorPosCallback(window, mouse_callback);
glfwSetCursorPos(window, 0.0, 0.0);
*/

class Camera
{
protected:
	// DebugCamera Angle
	float theta, phi;
	// DebugCamera Vectors for lookAt
	glm::vec3 cameraUp;
public:
    glm::vec3 cameraFront;
    bool in_use;

	Camera();
	~Camera();
    virtual void movement(double deltaTime, std::shared_ptr<GameEntity> chewy) = 0;
	glm::mat4 getViewMatrix();
    virtual void update_radius(float delta){};
	glm::vec3 cameraPosition;
};