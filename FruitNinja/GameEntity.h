#pragma once
#include "MeshSet.h"
#include <memory>
#include <vector>
#include "BoundingBox.h"

#define DRAW_MASK 0x01
#define OCTTREE_MASK 0x02
#define HIDE_MASK 0x04

#define CHECK_MASK(arg, mask) ((arg) & (mask))
#define SET_MASK(arg, mask) ((arg) | (mask))
#define CLEAR_MASK(arg, mask) ((arg) & (~(mask)))

#define SHOULD_DRAW(arg) CHECK_MASK(arg, DRAW_MASK)
#define IN_OCTTREE(arg) CHECK_MASK(arg, OCTTREE_MASK)
#define HIDE_BEHIND(arg) CHECK_MASK(arg, HIDE_MASK)

#define SET_DRAW(arg) SET_MASK(arg, DRAW_MASK)
#define SET_OCTTREE(arg) SET_MASK(arg, OCTTREE_MASK)
#define SET_HIDE(arg) SET_MASK(arg, HIDE_MASK)

#define UNSET_DRAW(arg) CLEAR_MASK(arg, DRAW_MASK)
#define UNSET_OCTTREE(arg) CLEAR_MASK(arg, OCTTREE_MASK)
#define UNSET_HIDE(arg) CLEAR_MASK(arg, HIDE_MASK)

class GameEntity
{
    float radius;
	glm::mat4 modelMat;
	bool validModelMat = false;
	glm::vec3 position;
	float scale = 1.0f;
	glm::vec3 rotations;
public:
    bool collision_response;
	std::shared_ptr<BoundingBox> largestBB;
    GameEntity() {}
	GameEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, bool collision_response = false);

    void sebInit();
	virtual void update();
    virtual void collision(std::shared_ptr<GameEntity> entity);

	glm::vec3 getPosition();
	void setPosition(glm::vec3 pos);
	float getScale();
	void setScale(float entScale);
	glm::vec3 getRotations();
	void setRotations(glm::vec3 rots);

	std::shared_ptr<MeshSet> mesh;
	glm::vec3 last_position;
	glm::vec3 turnAngle(glm::vec3 cartesian);

	glm::vec3 velocity = glm::vec3(0);
    virtual float getRadius();
    glm::vec3 getCenter();
	glm::vec3 center;
	aiAnimation *current_animation;
    bool compare(std::shared_ptr<GameEntity> ge);
    std::shared_ptr<std::vector<BoundingBox>> GameEntity::getTransformedBoundingBoxes();
    virtual glm::mat4 getModelMat();
	std::shared_ptr<BoundingBox> getOuterBoundingBox();
	std::shared_ptr<BoundingBox> getTransformedOuterBoundingBox();
	std::shared_ptr<BoundingBox> transformed_BB;
	char list = 0;
};