#include "ChewyAnimationComponent.h"
#include "GameEntity.h"
#include "World.h"

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
	if (static_cast<ChewyEntity*>(chewy)->moveComponent.archery_cam->in_use)
	{
		basicAnimation.changeToSingleAnimation(BOWPULL_START + bow_strength * BOWPULL_DURATION, BOWPULL_START + BOWPULL_DURATION);
		currentAnimtion = bowpull;
	}
	if (basicAnimation.animationComplete)
	{
		
		/*if (chewy->velocity.y < 15 && currentAnimtion == jumping && currentAnimtion)
		{
			basicAnimation.changeToSingleAnimation(FALLING_START, FALLING_START + FALLING_DURATION);
			currentAnimtion = falling;
		}
		else */if (chewy->velocity.y > 0 && currentAnimtion != jumping && currentAnimtion != falling)
		{
			basicAnimation.changeToSingleAnimation(JUMPING_START, JUMPING_START + JUMPING_DURATION);
			currentAnimtion = jumping;
		}
		else if (chewy->velocity.y == 0 && chewy->moving && currentAnimtion != walking)
		{
			basicAnimation.changeToLoopingAnimation(WALKING_START, WALKING_START + WALKING_DURATION);
			currentAnimtion = walking;
		}
		else if (chewy->velocity.y == 0 && !chewy->moving && currentAnimtion != standing)
		{
			basicAnimation.changeToLoopingAnimation(STANDING_START, STANDING_START + STANDING_DURATION);
			currentAnimtion = standing;
		}
	}
	basicAnimation.update();
}
