#pragma once

#include "Camera.h"
#include "CinematicPathingComponent.h"

class CinematicCamera : public Camera
{
public:
	CinematicCamera();
	~CinematicCamera();
	void init(glm::vec3 startLookAt, glm::vec3 center, std::vector<glm::vec3> control_points, float move_speed);
	void mouse_update();
	void movement(std::shared_ptr<GameEntity> chewy);
	glm::mat4 getViewMatrix();
	glm::vec3 center;
	glm::vec3 startLookAt;
	glm::vec3 lookAtPoint;
	glm::vec3 change;
	CinematicPathingComponent pathing;
};