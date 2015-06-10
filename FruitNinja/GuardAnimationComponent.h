#pragma once
#include "Component.h"
#include "BasicAnimationComponent.h"

#define WALKIN_START 20.0 / FRAMES_PER_SEC
#define WALKIN_DURATION 31.0 / FRAMES_PER_SEC
#define IDLE_START 60.0 / FRAMES_PER_SEC
#define IDLE_DURATION 50.0 / FRAMES_PER_SEC
#define DYING_START 120.0 / FRAMES_PER_SEC
#define DYING_DURATION 14.0 / FRAMES_PER_SEC

enum GAnimationState
{
	WALKIN,
	IDLE,
	DYING
};

class GuardAnimationComponent : public Component
{
public:
	void update() override;
private:
	GAnimationState currentAnimtion;
	GameEntity *guard;
public:
	GuardAnimationComponent(GameEntity* guard, GAnimationState state = IDLE);
	~GuardAnimationComponent();
	bool areYouDoneYet();
	void setCurrentAnimation(GAnimationState anim_st);
	BasicAnimationComponent basicAnimation;
};

