#pragma once
#include "Component.h"
#include "BasicAnimationComponent.h"

#define LOOK_AROUND_START 1.0 / FRAMES_PER_SEC
#define LOOK_AROUND_DURATION 39.0 / FRAMES_PER_SEC
#define STANDING_START LOOK_AROUND_START + LOOK_AROUND_DURATION + 1.0 / FRAMES_PER_SEC
#define STANDING_DURATION 95.0 / FRAMES_PER_SEC
#define WALKING_START STANDING_START + STANDING_DURATION + 1.0 / FRAMES_PER_SEC
#define WALKING_DURATION 21.0 / FRAMES_PER_SEC
#define JUMPING_START WALKING_START + WALKING_DURATION + 1.0 / FRAMES_PER_SEC
#define JUMPING_DURATION 3.0 / FRAMES_PER_SEC

enum AnimationState
{
	standing,
	looking_around,
	walking,
	jumping,
	falling, 
	landing
};

class ChewyAnimationComponent : public Component
{
public:
	void update() override;
private:
	BasicAnimationComponent basicAnimation;
	AnimationState currentAnimtion;
	GameEntity *chewy;
public:
	ChewyAnimationComponent(GameEntity* chewy);
	~ChewyAnimationComponent();
};

