#pragma once
#include "MeshSet.h"
#include <memory>
#include <vector>
#include "EntityBox.h"

#define DRAW_MASK 0x01
#define OCTTREE_MASK 0x02
#define HIDE_MASK 0x04
#define WALL_MASK 0x08

#define CHECK_MASK(arg, mask) ((arg) & (mask))
#define SET_MASK(arg, mask) ((arg) | (mask))
#define CLEAR_MASK(arg, mask) ((arg) & (~(mask)))

#define SHOULD_DRAW(arg) CHECK_MASK(arg, DRAW_MASK)
#define IN_OCTTREE(arg) CHECK_MASK(arg, OCTTREE_MASK)
#define HIDE_BEHIND(arg) CHECK_MASK(arg, HIDE_MASK)
#define IS_WALL(arg) CHECK_MASK(arg, WALL_MASK)

#define SET_DRAW(arg) SET_MASK(arg, DRAW_MASK)
#define SET_OCTTREE(arg) SET_MASK(arg, OCTTREE_MASK)
#define SET_HIDE(arg) SET_MASK(arg, HIDE_MASK)
#define SET_WALL(arg) SET_MASK(arg, WALL_MASK)

#define UNSET_DRAW(arg) CLEAR_MASK(arg, DRAW_MASK)
#define UNSET_OCTTREE(arg) CLEAR_MASK(arg, OCTTREE_MASK)
#define UNSET_HIDE(arg) CLEAR_MASK(arg, HIDE_MASK)
#define UNSET_WALL(arg) CLEAR_MASK(arg, WALL_MASK)

class GameEntity
{
	glm::mat4 modelMat;
	glm::mat4 alignedModelMat;
	bool validModelMat = false;
	bool validAlignedModelMat = false;
	float scale = 1.0f;
	glm::vec3 rotations;
public:
    EntityBox bounding_box;
    MeshSet* mesh;
    glm::vec3 last_position;
    glm::vec3 velocity = glm::vec3(0);
    aiAnimation *current_animation;
    char list = 0;

    bool collision_response;
	bool moving = false;

    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);

    GameEntity() {}
	GameEntity(glm::vec3 position, MeshSet* mesh, bool collision_response = false);

    void setup_entity_box();
    void setup_entity_box(MeshSet* mesh);
    void swap_bounding_box_width_depth();

	virtual void update();
    virtual void collision(GameEntity* entity);
    glm::vec3 turnAngle(glm::vec3 cartesian);
    virtual glm::mat4 getModelMat();
    glm::mat4 getAlignedModelMat();

	float getScale();
	void setScale(float entScale);

	glm::vec3 getRotations();
	void setRotations(glm::vec3 rots);
};