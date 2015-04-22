#include "PlayerCamera.h"
#include "World.h"
#include "ChewyEntity.h"

using namespace glm;

const float PlayerCamera::MinVerticalAngle = 0.0f;

/*
Default constructor.
*/
PlayerCamera::PlayerCamera()
{
	lookAtPoint = vec3(0);
    radius = 3.0f;
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
void PlayerCamera::mouse_update()
{
	float sensitivity = 0.1;

    theta += x_offset * sensitivity;
    phi += y_offset * sensitivity;

    if (phi > MaxVerticalAngle)
    {
        phi = MaxVerticalAngle;
        y_offset = 0;

    }
    if (phi < -MaxVerticalAngle)
    {
        phi = -MaxVerticalAngle;
        y_offset = 0;
    }

    cameraFront = rotateY(cameraFront, radians(x_offset * sensitivity));
    cameraFront = rotate(cameraFront, radians(y_offset * sensitivity), cross(cameraFront, cameraUp));
    cameraFront = normalize(cameraFront);
}

/**
Call this at the end of the draw loop to update for strafing.
*/
void PlayerCamera::movement(double deltaTime, std::shared_ptr<GameEntity> chewy)
{
    mouse_update();

    cameraPosition = chewy->position - radius * cameraFront;
    lookAtPoint = chewy->position;
}

mat4 PlayerCamera::getViewMatrix()
{
	return lookAt(cameraPosition, lookAtPoint, cameraUp);
}

void PlayerCamera::update_radius(float delta)
{
    radius += 0.1 * delta;
}