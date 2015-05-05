#pragma once
#include "Component.h"
#include "GameEntity.h"

class ChewyAnimationComponent :
	public Component
{
public:
	void update() override;
	ChewyAnimationComponent(GameEntity *chewy);
	~ChewyAnimationComponent();
private:
	aiNodeAnim* getBone(aiAnimation *animation, std::string boneName);

	GameEntity *entity;
	float frameTime;
};

