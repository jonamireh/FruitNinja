#pragma once

#include "Emitter.h"

#define POS_ATTRIB 0
#define FRAME_ATTRIB 1

class FlameEmitter : public Emitter {
public:
	FlameEmitter();
	~FlameEmitter();

	void update(double deltaTime, std::vector<Light*> lights, bool do_stuff = false);
private:
	int prev_size = 0;
};