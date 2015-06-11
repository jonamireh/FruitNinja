#include "ExplosionEmitter.h"
#include <random>

std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(-PARTICLE_SPEED, PARTICLE_SPEED);

ExplosionEmitter::ExplosionEmitter(std::string filename, double tframe, int num_cols, int num_rows,
	float part_w, float part_h, int num_parts, glm::vec3 accel) 
	: Emitter(filename, tframe, num_cols, num_rows, part_w, part_h)
{
	acceleration = accel;
	num_exp_particles = num_parts;
}

ExplosionEmitter::~ExplosionEmitter() {

}

void ExplosionEmitter::update(double deltaTime) {
	time_uniform += deltaTime;
	for (int i = 0; i < explosions.size(); i++) {
		explosions[i] += deltaTime;
	}
	while (!explosions.empty() && explosions.front() >= max_time) {
		explosions.pop_front();
		particles.position.erase(particles.position.begin(), particles.position.begin() + num_exp_particles);
		particles.velocity.erase(particles.velocity.begin(), particles.velocity.begin() + num_exp_particles);
		particles.frame.erase(particles.frame.begin(), particles.frame.begin() + num_exp_particles);
	}

	if (explosions.size() != prev_num_exp) {
		if (getNumParticles() > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
			glBufferData(GL_ARRAY_BUFFER, particles.position.size() * sizeof(glm::vec3), &particles.position[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, vel_VBO);
			glBufferData(GL_ARRAY_BUFFER, particles.velocity.size() * sizeof(glm::vec3), &particles.velocity[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
			glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(glm::vec3), &particles.frame[0], GL_STATIC_DRAW);
		}
		prev_num_exp = explosions.size();
	}
}

void ExplosionEmitter::add(glm::vec3 pos)
{
	for (int i = 0; i < particles.position.size(); i++) {
		particles.position[i] += particles.velocity[i] * time_uniform + acceleration * time_uniform * time_uniform;
		particles.frame[i] += time_uniform / frame_time;
	}

	explosions.push_back(0.0);
	for (int i = 0; i < num_exp_particles; i++) {
		particles.position.push_back(pos);
		particles.frame.push_back(0);
		particles.velocity.push_back(glm::vec3(distribution(generator), distribution(generator)/2.0, distribution(generator)));
	}
	time_uniform = 0.0;
	std::cout << "BOOM!\n";
}