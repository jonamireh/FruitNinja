#include "FlameEmitter.h"

FlameEmitter::FlameEmitter() : Emitter("nice_fire.png", 36, .03) {

}

FlameEmitter::~FlameEmitter() {

}

void FlameEmitter::update(double deltaTime, std::vector<Light*> lights) {
	//if the size of the lights vector changed reset particles
	static int prev_size = 0;
	if (prev_size != lights.size()) {
		particles.position.clear();
		particles.frame.clear();
		if (lights.size() == 0) {
			//again I have to be drawing a particle to not crash
			particles.position.push_back(glm::vec3(-200, -200, -200));
			particles.frame.push_back(0);
		}
		for (int i = 0; i < lights.size(); i++) {
			particles.position.push_back(lights[i]->pos + glm::vec3(0.0, -1.4, 0.0));
			particles.frame.push_back(rand() % num_frames);
		}
		glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
		glBufferData(GL_ARRAY_BUFFER, particles.position.size() * sizeof(glm::vec3), &particles.position[0], GL_STATIC_DRAW);
		prev_size = lights.size();
	}

	//glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	//glBufferData(GL_ARRAY_BUFFER, particles.midpt.size() * sizeof(glm::vec3), &particles.midpt[0], GL_DYNAMIC_DRAW);

	Emitter::update(deltaTime);
}
