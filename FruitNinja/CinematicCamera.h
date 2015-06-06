#pragma once

#include "Camera.h"
#include "CinematicPathingComponent.h"

class CinematicCamera : public Camera
{
public:
	CinematicCamera();
	~CinematicCamera();
	void init(std::vector<glm::vec3> position_points, std::vector<glm::vec3> lookAt_points, float move_speed);
	void mouse_update();
	void movement(GameEntity* chewy) override;
	glm::mat4 getViewMatrix();

	CinematicPathingComponent pathing;
private:
	glm::vec3 lookAtPoint;
};