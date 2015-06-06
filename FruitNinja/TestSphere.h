#pragma once
#include "GameEntity.h"
#include "BasicAnimationComponent.h"

class TestSphere :
	public GameEntity
{
	BasicAnimationComponent animComponent;
public:
	void update() override;
	TestSphere(MeshSet* mesh);
	~TestSphere();
};

