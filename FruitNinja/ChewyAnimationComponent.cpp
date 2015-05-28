#include "ChewyAnimationComponent.h"
#include "GameEntity.h"

ChewyAnimationComponent::ChewyAnimationComponent(GameEntity* chewy) : basicAnimation(chewy)
{
	basicAnimation.changeToAnimationBlock(STANDING_START, STANDING_START + STANDING_DURATION);
	currentAnimtion = standing;
	this->chewy = chewy;
}


ChewyAnimationComponent::~ChewyAnimationComponent()
{
}

void ChewyAnimationComponent::update()
{
	if (chewy->moving && currentAnimtion != walking)
	{
		basicAnimation.changeToAnimationBlock(WALKING_START, WALKING_START + WALKING_DURATION);
		currentAnimtion = walking;
	}
	else if (!chewy->moving && currentAnimtion != standing)
	{
		basicAnimation.changeToAnimationBlock(STANDING_START, STANDING_START + STANDING_DURATION);
		currentAnimtion = standing;
	}
	basicAnimation.update();
}
