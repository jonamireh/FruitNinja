#pragma once
#include "Camera.h"
class StaticCamera :
	public Camera
{
public:
	void movement(GameEntity* chewy) override;
public:
	StaticCamera(glm::vec3 position, glm::vec3 look_at_point);
	~StaticCamera();
};

