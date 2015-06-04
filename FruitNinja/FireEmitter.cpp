#include "FireEmitter.h"
#include <random>

std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0.0, 1.0);

FireEmitter::FireEmitter() : Emitter("fire_smoke_atlas.png", 32, .03, 4, 8, 5.0) {
	particles.frame.clear();
	particles.position.clear();
	for (int i = 0; i < 2; i++) {
		particles.position.push_back(glm::vec3(50.0 + i * 5.0, 5.0, 50.0));
		particles.frame.push_back(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.position.size() * sizeof(glm::vec3), &particles.position[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(int), &particles.frame[0], GL_DYNAMIC_DRAW);
}

FireEmitter::~FireEmitter() {
}

void FireEmitter::update(double deltaTime) {
	static double prev_frame_time = 0.0;
	static double fuck = 0.0;
	fuck += deltaTime;
	prev_frame_time += deltaTime;
	if (prev_frame_time > frame_time) {
		int inc = prev_frame_time / frame_time; //integer divide tells how many frames pass if any
		prev_frame_time -= inc * frame_time; //should keep remainder
		for (int i = 0; i < particles.frame.size(); i++) {
			particles.frame[i] += inc;
			particles.position[i] += glm::vec3(distribution(generator) / 8.0, .1, distribution(generator) / 8.0) * (float)inc;
			if (particles.frame[i] >= num_frames) {
				particles.position.erase(particles.position.begin() + i);
				particles.frame.erase(particles.frame.begin() + i);
			}
		}
		if (fuck > 2.0) {
			particles.position.push_back(glm::vec3(50.0, 1.8, 50.0));
			particles.frame.push_back(16);
			particles.position.push_back(glm::vec3(54.0, 1.7, 50.0));
			particles.frame.push_back(rand() % 4);
			particles.position.push_back(glm::vec3(50.0, 2.2, 54.0));
			particles.frame.push_back(rand() % 4);
			particles.position.push_back(glm::vec3(54.0, 2.1, 54.0));
			particles.frame.push_back(rand() % 4);
			fuck = 0.0;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.position.size() * sizeof(glm::vec3), &particles.position[0], GL_DYNAMIC_DRAW);


	//not really sure how to do this on the gpu so I just send it every frame...
	glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(int), &particles.frame[0], GL_DYNAMIC_DRAW);
}