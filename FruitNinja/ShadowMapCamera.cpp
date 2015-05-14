#include "ShadowMapCamera.h"

using namespace glm;

ShadowMapCamera::ShadowMapCamera(vec3 position, vec3 front, vec3 up)
{
	cameraPosition = position;
	cameraFront = front;
	cameraUp = up;
}

ShadowMapCamera::~ShadowMapCamera()
{

}