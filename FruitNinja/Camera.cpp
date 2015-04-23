#include "Camera.h"

using namespace glm;

Camera::Camera()
{
    in_use = false;
	theta = 0.0f;
	phi = 0.0f;
	cameraPosition = vec3(0.0f, 5.0f, 5.0f);
	cameraFront = vec3(0.0f, 0.0f, -1.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
}


Camera::~Camera()
{

}

mat4 Camera::getViewMatrix()
{
	return lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}