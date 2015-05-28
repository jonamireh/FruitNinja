#include "ChewyAnimationComponent.h"
#include "GameEntity.h"

ChewyAnimationComponent::ChewyAnimationComponent(GameEntity* chewy) : basicAnimation(chewy)
{
	basicAnimation.changeToLoopingAnimation(STANDING_START, STANDING_START + STANDING_DURATION);
	currentAnimtion = standing;
	this->chewy = chewy;
}


ChewyAnimationComponent::~ChewyAnimationComponent()
{
}

void ChewyAnimationComponent::update()
{
	if (chewy->velocity.y > 0 && currentAnimtion != jumping)
	{
		basicAnimation.changeToSingleAnimation(JUMPING_START, JUMPING_START + JUMPING_DURATION);
		currentAnimtion = jumping;
	}
	else if (chewy->moving && currentAnimtion != walking)
	{
		basicAnimation.changeToLoopingAnimation(WALKING_START, WALKING_START + WALKING_DURATION);
		currentAnimtion = walking;
	}
	else if (!chewy->moving && currentAnimtion != standing)
	{
		basicAnimation.changeToLoopingAnimation(STANDING_START, STANDING_START + STANDING_DURATION);
		currentAnimtion = standing;
	}
	basicAnimation.update();
}
