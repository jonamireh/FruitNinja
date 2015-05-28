#include "GuardAnimationComponent.h"
#include "GameEntity.h"
#include "GuardEntity.h"

GuardAnimationComponent::GuardAnimationComponent(GameEntity* guard) : basicAnimation(guard)
{
	//basicAnimation.changeToLoopingAnimation(WALKIN_START, WALKIN_START + WALKIN_DURATION);
	currentAnimtion = WALKIN;
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
