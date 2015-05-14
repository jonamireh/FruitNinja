#pragma once

#include "Camera.h"

class ShadowMapCamera : public Camera
{
public:
	ShadowMapCamera(glm::vec3 position, glm::vec3 front, glm::vec3 up);
	~ShadowMapCamera();
	void movement(std::shared_ptr<GameEntity> chewy) {};
};