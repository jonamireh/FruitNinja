#include "ChewyAnimationComponent.h"
#include "GameEntity.h"
#include "World.h"
#include "main.h"

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
	float static standingTime = 0;
	standingTime += seconds_passed;
	if (currentAnimtion != ending)
	{
		if (world->endScene != nullptr && currentAnimtion != ending)
		{
			basicAnimation.changeToSingleAnimation(ENDING_START, ENDING_START + ENDING_DURATION);
			currentAnimtion = ending;
		}
		if (static_cast<ChewyEntity*>(chewy)->moveComponent.archery_cam->in_use)
		{
			basicAnimation.changeToSingleAnimation(BOWPULL_START + bow_strength * BOWPULL_DURATION, BOWPULL_START + BOWPULL_DURATION);
			currentAnimtion = bowpull;
		}
		if (basicAnimation.animationComplete && currentAnimtion != caught && currentAnimtion != death)
		{

			if (static_cast<ChewyEntity*>(chewy)->isDead && currentAnimtion != death)
			{
				basicAnimation.changeToSingleAnimation(DEATH_START, DEATH_START + DEATH_DURATION);
				currentAnimtion = death;
			}
			else if (currentAnimtion == standing && standingTime >= 15.0f)
			{
				basicAnimation.changeToSingleAnimation(BOWPULL_START + bow_strength * BOWPULL_DURATION, BOWPULL_START + BOWPULL_DURATION);
				currentAnimtion = bowpull;
			}
			if (basicAnimation.animationComplete && currentAnimtion != caught)
			{

				/*if (chewy->velocity.y < 15 && currentAnimtion == jumping && currentAnimtion)
				{
				basicAnimation.changeToSingleAnimation(FALLING_START, FALLING_START + FALLING_DURATION);
				currentAnimtion = falling;
				}
				else */if (currentAnimtion == standing && standingTime >= 15.0f)
				{
					standingTime = 0;
					basicAnimation.changeToSingleInterruptibleAnimation(LOOK_AROUND_START, LOOK_AROUND_START + LOOK_AROUND_DURATION);
					currentAnimtion = looking_around;
				}
				else if (static_cast<ChewyEntity*>(chewy)->isCaught && currentAnimtion != caught) {
					basicAnimation.changeToSingleAnimation(CAUGHT_START, CAUGHT_START + CAUGHT_DURATION);
					currentAnimtion = caught;
				}
				else if (chewy->velocity.y > 0 && currentAnimtion != jumping && currentAnimtion != falling)
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
					if (currentAnimtion == looking_around && standingTime >= LOOK_AROUND_DURATION || currentAnimtion != looking_around)
					{
						standingTime = 0;
						basicAnimation.changeToLoopingAnimation(STANDING_START, STANDING_START + STANDING_DURATION);
						currentAnimtion = standing;
					}
				}
			}
		}
	}
	basicAnimation.update();
}
