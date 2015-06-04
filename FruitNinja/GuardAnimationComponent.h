#pragma once
#include "Component.h"
#include "BasicAnimationComponent.h"

#define WALKIN_START 20.0 / FRAMES_PER_SEC
#define WALKIN_DURATION 31.0 / FRAMES_PER_SEC


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

