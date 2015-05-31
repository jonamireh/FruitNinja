#pragma once
#include "Emitter.h"

class FireEmitter : public Emitter {
public:
	FireEmitter();
	~FireEmitter();
	void update(double deltaTime) override;
};