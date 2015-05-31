#include "FireEmitter.h"

FireEmitter::FireEmitter() : Emitter("fire_atlas.png", 16, .05, 4, 4, 5.0) {
	particles.frame.clear();
	particles.position.clear();
	for (int i = 0; i < 2; i++) {
		particles.position.push_back(glm::vec3(50.0 + i * 5.0, 5.0, 50.0));
		particles.frame.push_back(rand() % num_frames);
	}
	glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.position.size() * sizeof(glm::vec3), &particles.position[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(int), &particles.frame[0], GL_DYNAMIC_DRAW);
}

FireEmitter::~FireEmitter() {

}

void FireEmitter::update(double deltaTime) {
	Emitter::update(deltaTime);
}