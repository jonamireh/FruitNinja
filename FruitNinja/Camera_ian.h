#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>
using namespace glm;

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
	// DebugCamera Location
	double xPos, yPos, xPrev, yPrev;
	// DebugCamera Angle
	float theta, phi;
	// DebugCamera Vectors for lookAt
	vec3 cameraPosition;
	vec3 cameraFront;
	vec3 cameraUp;
	// Key Map for Strafing
	bool keys[1024];
public:
	Camera();
	~Camera();
	virtual void cursorPosCallback(double, double) = 0;
	virtual void keyCallback(int, int, int, int) = 0;
	virtual void movement(double) = 0;
	mat4 getViewMatrix();
};