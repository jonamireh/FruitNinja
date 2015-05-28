#include "DebugCamera.h"
#include "World.h"
#include <glm/gtx/string_cast.hpp>

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
void DebugCamera::mouse_update()
{
	float sensitivity = 0.1;

    theta += x_offset * sensitivity;
    phi += y_offset * sensitivity;

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
void DebugCamera::movement(shared_ptr<GameEntity> chewy)
{
    float cameraSpeed = 1.;

	if (keys[GLFW_KEY_W])
		cameraPosition += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPosition -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPosition -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPosition += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

	cout << glm::to_string(cameraPosition) << endl;

    // NOW DO MOUSE ADJUSTS!
    mouse_update();
}