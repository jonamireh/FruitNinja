#include "FireEmitter.h"

FireEmitter::FireEmitter() : Emitter("nice_fire.png", .02, 6, 6, 3.0, 3.0)
{
	acceleration = glm::vec3(0.0, -GRAVITY, 0.0);
}

FireEmitter::~FireEmitter() {
}

void FireEmitter::update(double deltaTime, std::vector<FireArrowEntity*> fireArrows)
{
	if (prev_arrows != fireArrows.size()) {
		time_uniform = 0.0f;
		particles.position.clear();
		particles.velocity.clear();
		particles.frame.clear();

		for (int i = 0; i < fireArrows.size(); i++) {
			particles.position.push_back(fireArrows[i]->getPosition() - glm::vec3(0.0, particle_height / 2.0, 0.0));
			particles.velocity.push_back(fireArrows[i]->getVelocity());
			particles.frame.push_back(0);
		}
		if (getNumParticles() > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
			glBufferData(GL_ARRAY_BUFFER, particles.position.size() * sizeof(glm::vec3), &particles.position[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, vel_VBO);
			glBufferData(GL_ARRAY_BUFFER, particles.velocity.size() * sizeof(glm::vec3), &particles.velocity[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
			glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(glm::vec3), &particles.frame[0], GL_STATIC_DRAW);
		}
		prev_arrows = fireArrows.size();
	}
	time_uniform += deltaTime;
}