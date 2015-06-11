#pragma once

#include "Particle.h"

#define POS_ATTRIB 0
#define VEL_ATTRIB 1
#define FRAME_ATTRIB 2

//I think an SOA is better than AOS for gpu...
struct Particles {
	std::vector<glm::vec3> position;
	std::vector<glm::vec3> velocity;
	std::vector<int> frame;
};

class Emitter {
protected:
	glm::vec3 center;
	Particles particles;
	GLuint pos_VBO;
	GLuint vel_VBO;
	GLuint frame_VBO;
	int num_frames;
	float max_time;
public:
	Emitter(string filename, float tframe, int width, int height, float part_w, float part_h);
	virtual ~Emitter();

	virtual void update(double deltaTime);
	int getNumParticles();
	tdogl::Texture* texture;

	int rows;
	int columns;
	float particle_width;
	float particle_height;
	float time_uniform;
	float frame_time;
	glm::vec3 acceleration;

	GLuint VAO;
};