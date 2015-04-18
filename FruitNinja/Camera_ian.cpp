#include "Camera.h"

Camera::Camera()
{
	xPos = 0.0f;
	yPos = 0.0f;
	xPrev = 0.0f;
	yPrev = 0.0f;
	theta = 0.0f;
	phi = 0.0f;
	cameraPosition = vec3(0.0f, 0.0f, 3.0f);
	cameraFront = vec3(0.0f, 0.0f, 1.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
	keys[1024] = { false };
}

Camera::~Camera()
{

}

mat4 Camera::getViewMatrix()
{
	return lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}