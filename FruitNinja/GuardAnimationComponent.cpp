#include "GuardAnimationComponent.h"
#include "GameEntity.h"
#include "GuardEntity.h"

GuardAnimationComponent::GuardAnimationComponent(GameEntity* guard, GAnimationState state) : basicAnimation(guard)
{
	currentAnimtion = state;
	if (currentAnimtion == WALKIN) {
		basicAnimation.changeToLoopingAnimation(WALKIN_START, WALKIN_START + WALKIN_DURATION);
	} else {
		basicAnimation.changeToLoopingAnimation(IDLE_START, IDLE_START + IDLE_DURATION);
	}
	this->guard = guard;
}


GuardAnimationComponent::~GuardAnimationComponent()
{
}

void GuardAnimationComponent::update()
{
	if (basicAnimation.animationComplete)
	{
		//basicAnimation.changeToSingleAnimation(WALKIN_START, WALKIN_START + WALKIN_DURATION);
	}
	basicAnimation.update();
}
