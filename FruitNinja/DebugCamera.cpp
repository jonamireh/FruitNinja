#include "DebugCamera.h"
#include "Global.h"
#include "World.h"

using namespace glm;


/*
	Default constructor.
*/
DebugCamera::DebugCamera()
{

}

/*
	Destructor.
*/
DebugCamera::~DebugCamera()
{

}

/*
Call the glfwSetCursorPosCallback to the window and this function.
*/
void DebugCamera::cursorPosCallback(double xpos, double ypos)
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
Call this at the end of the draw loop to update for strafing.
*/
void DebugCamera::movement(double deltaTime, std::shared_ptr<GameEntity> chewy)
{
    float cameraSpeed = 0.01;

	if (keys[GLFW_KEY_W])
		cameraPosition += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPosition -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPosition -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPosition += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
}