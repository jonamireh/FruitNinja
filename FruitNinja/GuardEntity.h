#pragma once

#include "GameEntity.h"
#include "GuardMovementComponent.h"
#include "GuardAnimationComponent.h"
#include "FrustrumCulling.h"
#include "AudioManager.h"
#include "GuardPuppeteer.h"

class GuardEntity : public GameEntity
{
public:
	~GuardEntity();
	//GuardEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points, float move_speed, bool linear_curve = false);
	//GuardEntity(glm::vec3 position, MeshSet* mesh, glm::vec3 dir);

	GuardEntity(glm::vec3 position, MeshSet* mesh, GuardPuppeteer* puppeteer, std::vector<glm::vec3> control_points, float move_speed, bool linear_curve = false, bool armored = false);
	GuardEntity(glm::vec3 position, MeshSet* mesh, GuardPuppeteer* puppeteer, glm::vec3 dir, bool armored = false);


    void collision(GameEntity* entity);
	void update();
	bool check_view(ChewyEntity* chewy, std::vector<GameEntity*> entities);
	void stopWalkSound();
	void goAheadAndKillYourself();
	glm::vec3 front;
	GuardMovementComponent move_component;
	GuardAnimationComponent animComponent;
	bool is_armored;
	std::vector<std::vector<glm::mat4>>* getBoneTransformations() override;
	void stopMoving();
	bool static_movement = false;
private:
	bool is_dying = false;

	FMOD::Channel *walk_channel;

	void playWalkSound();
	GuardPuppeteer* _puppeteer;
};