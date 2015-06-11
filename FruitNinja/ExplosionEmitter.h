#pragma once
#include "Emitter.h"
#include <deque>

#define PARTICLE_SPEED 50.0f

class ExplosionEmitter : public Emitter {
public:
	ExplosionEmitter(std::string filename, double tframe, int num_cols, int num_rows,
		float part_w, float part_h, int num_parts = 100, glm::vec3 accel = glm::vec3(0.0, -GRAVITY, 0.0));
	virtual ~ExplosionEmitter();
	void update(double deltaTime) override;
	void add(glm::vec3 pos);
private:
	int prev_num_exp = 0;
	std::deque<double> explosions;
	int num_exp_particles;
};