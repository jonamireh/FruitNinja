#include "FlameEmitter.h"

FlameEmitter::FlameEmitter() : Emitter("nice_fire.png", .03, 6, 6, 5.0, 5.0) {

}

FlameEmitter::~FlameEmitter() {

}

void FlameEmitter::update(double deltaTime, std::vector<Light*> lights) {
	//if the size of the lights vector changed reset particles
	if (prev_size != lights.size()) {
		particles.position.clear();
		particles.velocity.clear();
		particles.frame.clear();

		for (int i = 0; i < lights.size(); i++) {
			particles.position.push_back(lights[i]->pos + glm::vec3(0.0, -2.2, 0.0));
			particles.velocity.push_back(glm::vec3(0.0, 0.0, 0.0));
			particles.frame.push_back(rand() % num_frames);
		}
		if (getNumParticles() > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
			glBufferData(GL_ARRAY_BUFFER, particles.position.size() * sizeof(glm::vec3), &particles.position[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, vel_VBO);
			glBufferData(GL_ARRAY_BUFFER, particles.velocity.size() * sizeof(glm::vec3), &particles.velocity[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
			glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(glm::vec3), &particles.frame[0], GL_STATIC_DRAW);
			prev_size = lights.size();
		}
	}

	Emitter::update(deltaTime);
}
