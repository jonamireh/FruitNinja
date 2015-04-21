#include "Camera.h"

using namespace glm;

Camera::Camera()
{
	xPos = 0.0f;
	yPos = 0.0f;
	xPrev = 0.0f;
	yPrev = 0.0f;
	theta = 0.0f;
	phi = 0.0f;
	cameraPosition = vec3(0.0f, 0.0f, 3.0f);
	cameraFront = vec3(0.0f, 0.0f, -1.0f);
	cameraUp = vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera()
{

}

mat4 Camera::getViewMatrix()
{
    std::cout << "camera x: " << cameraPosition.x << " y: " << cameraPosition.y << " z: " << cameraPosition.z << std::endl;
	return lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}