#pragma once
#include "Emitter.h"
#include "FireArrowEntity.h"

class FireEmitter : public Emitter {
public:
	FireEmitter();
	~FireEmitter();
	void update(double deltaTime, std::vector<FireArrowEntity*> fireArrows);
};