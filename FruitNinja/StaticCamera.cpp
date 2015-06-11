#include "StaticCamera.h"


void StaticCamera::movement(GameEntity* chewy)
{
	// Why the hell is this required
}

StaticCamera::StaticCamera(glm::vec3 position, glm::vec3 look_at_point)
{
	cameraPosition = position;
	cameraFront = look_at_point - position;
}


StaticCamera::~StaticCamera()
{
}
