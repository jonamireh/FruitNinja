#pragma once

#include "Particle.h"

#define POS_ATTRIB 0
#define FRAME_ATTRIB 1

//I think an SOA is better than AOS for gpu...
struct Particles {
	std::vector<glm::vec3> position;
	std::vector<int> frame;
};

class Emitter {
protected:
	glm::vec3 center;
	Particles particles;
	GLuint pos_VBO;
	GLuint frame_VBO;
	int num_frames;
	double frame_time;
public:
	Emitter(string filename, int nframes, double tframe, int width, int height, float size);
	virtual ~Emitter();

	virtual void update(double deltaTime);
	int getNumParticles();
	tdogl::Texture* texture;

	int atlas_height;
	int atlas_width;
	float particle_size;

	GLuint VAO;
};