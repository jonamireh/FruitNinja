#pragma once
#include "MeshSet.h"
#include <memory>
#include <vector>
#include "BoundingBox.h"

class GameEntity
{
    void sebInit();
	std::shared_ptr<BoundingBox> largestBB;
public:
    GameEntity() {}
	GameEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : position(position), mesh(mesh), rotations(glm::vec3(0.f, 0.f, 0.f))
    {
		sebInit();
    };
	virtual void update() = 0;
    virtual void collisionBB(std::shared_ptr<GameEntity> entity);
	virtual void collisionBS(std::shared_ptr<GameEntity> entity);
	std::shared_ptr<MeshSet> mesh;
	glm::vec3 position;
	glm::vec3 last_position;
    glm::vec3 rotations;
    float scale = 1.0f;
	glm::vec3 turnAngle(glm::vec3 cartesian);
	glm::vec3 velocity = glm::vec3(0);
    virtual float getRadius();
	float radius;
    virtual glm::vec3 getCenter();
	glm::vec3 center;
    bool compare(std::shared_ptr<GameEntity> ge);
    std::shared_ptr<std::vector<BoundingBox>> GameEntity::getTransformedBoundingBoxes();
    virtual glm::mat4 getModelMat();
	virtual std::shared_ptr<BoundingBox> getOuterBoundingBox();
	virtual std::shared_ptr<BoundingBox> getTransformedOuterBoundingBox();
};