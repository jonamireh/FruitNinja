#include "PlayerCamera.h"
#include "World.h"

using namespace glm;

const float PlayerCamera::MinVerticalAngle = 0.0f;

/*
Default constructor.
*/
PlayerCamera::PlayerCamera()
{
	lookAtPoint = vec3(0);
}

/*
Specify the lookAt point (i.e. the player character)
*/
PlayerCamera::PlayerCamera(vec3 lookAt)
{
	this->lookAtPoint = lookAt;
}

/*
Destructor.
*/
PlayerCamera::~PlayerCamera()
{

}

/*
Call the glfwSetCursorPosCallback to the window and this function.
*/
void PlayerCamera::cursorPosCallback(double xpos, double ypos)
{
	GLfloat xoffset = xpos - xPrev;
	GLfloat yoffset = yPrev - ypos;
	xPrev = xpos;
	yPrev = ypos;

	GLfloat sensitivity = 0.1;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	theta += xoffset;
	double tempPhi = phi;
	phi += yoffset;

	if (phi > MaxVerticalAngle)
		phi = MaxVerticalAngle;
	if (phi < MinVerticalAngle)
		phi = -MinVerticalAngle;

	yoffset = phi - tempPhi;

	cameraPosition.x += cos(radians(xoffset)) * cos(radians(yoffset));
	cameraPosition.y += sin(radians(yoffset));
	cameraPosition.z += sin(radians(xoffset)) * cos(radians(yoffset));
}

/**
Call this at the end of the draw loop to update for strafing.
*/
void PlayerCamera::movement(double deltaTime)
{
	//float cameraSpeed = 0.01;
	//vec3 xzDisplacement = vec3(cameraFront.x, 0, cameraFront.z);
	//if (keys[GLFW_KEY_W])
	//{
	//	vec3 speedDisplacement = cameraSpeed * xzDisplacement;
	//	cameraPosition += speedDisplacement;
	//	lookAtPoint += speedDisplacement;
	//}
	//if (keys[GLFW_KEY_S])
	//{
	//	vec3 speedDisplacement = cameraSpeed * xzDisplacement;
	//	cameraPosition -= speedDisplacement;
	//	lookAtPoint -= speedDisplacement;
	//}
	//if (keys[GLFW_KEY_A])
	//{
	//	vec3 speedDisplacement = normalize(cross(xzDisplacement, cameraUp)) * cameraSpeed;
	//	cameraPosition -= speedDisplacement;
	//	lookAtPoint -= speedDisplacement;
	//}
	//if (keys[GLFW_KEY_D])
	//{
	//	vec3 speedDisplacement = normalize(cross(xzDisplacement, cameraUp)) * cameraSpeed;
	//	cameraPosition += speedDisplacement;
	//	lookAtPoint += speedDisplacement;
	//}
}

mat4 PlayerCamera::getViewMatrix()
{
	return lookAt(cameraPosition, cameraPosition - lookAtPoint, cameraUp);
}