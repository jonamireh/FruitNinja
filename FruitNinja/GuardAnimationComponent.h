#pragma once
#include "Component.h"
#include "BasicAnimationComponent.h"

#define WALKIN_START 1.0 / FRAMES_PER_SEC
#define WALKIN_DURATION 200.0 / FRAMES_PER_SEC


enum GAnimationState
{
	WALKIN
};

class GuardAnimationComponent : public Component
{
public:
	void update() override;
private:
	BasicAnimationComponent basicAnimation;
	GAnimationState currentAnimtion;
	GameEntity *guard;
public:
	GuardAnimationComponent(GameEntity* guard);
	~GuardAnimationComponent();
};

