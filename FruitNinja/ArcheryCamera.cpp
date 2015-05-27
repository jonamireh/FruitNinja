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
	cameraFront = front;//normalize(front); //not necessary, already normalized
}

/**
Call this at the end of the draw loop to update for strafing.
*/
void ArcheryCamera::movement(shared_ptr<GameEntity> chewy) {
    mouse_update();

    // Should set the Archery Camera to be at chewy but slightly to the right?
	cameraPosition = chewy->getPosition() + glm::rotateY(vec3(-1.5f, 4.f, -3.f), chewy->turnAngle(cameraFront).y);
}