#include "ArcheryCamera.h"
#include <GL/glew.h>
#include "Global.h"
#include "World.h"
#include "ChewyEntity.h"

using namespace glm;

/*
Default constructor.
*/
ArcheryCamera::ArcheryCamera()
{

}

/*
Destructor.
*/
ArcheryCamera::~ArcheryCamera()
{

}

/*
Call the glfwSetCursorPosCallback to the window and this function.
*/
void ArcheryCamera::mouse_update()
{
	float xoffset = x_offset;
	float yoffset = y_offset;

	GLfloat sensitivity = 0.1;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	theta += xoffset;
	phi += yoffset;

	if (phi > MaxVerticalAngle)
		phi = MaxVerticalAngle;
	if (phi < -MaxVerticalAngle)
		phi = -MaxVerticalAngle;

	vec3 front;
	front.x = cos(radians(theta)) * cos(radians(phi));
	front.y = sin(radians(phi));
	front.z = sin(radians(theta)) * cos(radians(phi));
	cameraFront = normalize(front);
}

/**
Call this at the end of the draw loop to update for strafing.
*/
void ArcheryCamera::movement(double deltaTime, shared_ptr<GameEntity> chewy)
{
	float cameraSpeed = 0.01;
	vec3 xzDisplacement = vec3(cameraFront.x, 0, cameraFront.z);
	if (keys[GLFW_KEY_W])
		cameraPosition += cameraSpeed * xzDisplacement;
	if (keys[GLFW_KEY_S])
		cameraPosition -= cameraSpeed * xzDisplacement;
	if (keys[GLFW_KEY_A])
		cameraPosition -= normalize(cross(xzDisplacement, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPosition += normalize(cross(xzDisplacement, cameraUp)) * cameraSpeed;

    mouse_update();
}