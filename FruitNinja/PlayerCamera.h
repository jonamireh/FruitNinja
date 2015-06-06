#pragma once

#include "Camera.h"
#include "GameEntity.h"
#include <glm/gtx/rotate_vector.hpp>

class PlayerCamera : public Camera
{
	static const float MinVerticalAngle;
    
    float radius;

public:
	PlayerCamera();
	PlayerCamera(glm::vec3 lookAt);
	~PlayerCamera();
    void mouse_update();
    void movement(GameEntity* chewy) override;
    glm::mat4 getViewMatrix();
    void update_radius(float delta);
	void reorient(std::vector<GameEntity*> entities, ChewyEntity* chewy);
	glm::vec3* PlayerCamera::get_near_plane_corners();
	glm::vec3 handle_collision_zoom(float minOffsetDist, glm::vec3* frustumNearCorners, std::vector<GameEntity*> entities, ChewyEntity* chewy);
	glm::vec3 lookAtPoint;
};