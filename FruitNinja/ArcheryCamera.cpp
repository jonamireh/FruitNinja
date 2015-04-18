#include "ArcheryCamera.h"

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
void ArcheryCamera::cursorPosCallback(double xpos, double ypos)
{
	GLfloat xoffset = xpos - xPrev;
	GLfloat yoffset = yPrev - ypos;
	xPrev = xpos;
	yPrev = ypos;

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
Call the glfwSetKeyCallback to the window and this function.
*/
void ArcheryCamera::keyCallback(int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

/**
Call this at the end of the draw loop to update for strafing.
*/
void ArcheryCamera::movement(double deltaTime)
{
	float cameraSpeed = 0.006f * deltaTime;
	vec3 xzDisplacement = vec3(cameraFront.x, 0, cameraFront.z);
	if (keys[GLFW_KEY_W])
		cameraPosition += cameraSpeed * xzDisplacement;
	if (keys[GLFW_KEY_S])
		cameraPosition -= cameraSpeed * xzDisplacement;
	if (keys[GLFW_KEY_A])
		cameraPosition -= normalize(cross(xzDisplacement, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPosition += normalize(cross(xzDisplacement, cameraUp)) * cameraSpeed;
}