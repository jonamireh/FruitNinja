#pragma once
#include "MeshSet.h"
#include "Material.h"
#include <memory>

class GameEntity
{
    void sebInit();
    float radius;
    glm::vec3 center;
public:
    GameEntity() {}
    GameEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : position(position), mesh(mesh), rotations(glm::vec3(0.f, 0.f, 0.f))
    {
        sebInit();
    }
	virtual void update() =0;
	std::shared_ptr<MeshSet> mesh;
	glm::vec3 position;
    glm::vec3 rotations;
    float scale = 1.0f;
	glm::vec3 turnAngle(glm::vec3 cartesian);
	glm::vec3 velocity = glm::vec3(0);
    float getRadius();
    glm::vec3 getCenter();
};