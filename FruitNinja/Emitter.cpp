#include "Emitter.h"

Emitter::Emitter(string filename, float tframe, int cols, int rows, float part_w, float part_h)
	: num_frames(cols * rows), frame_time(tframe), columns(cols), rows(rows), 
	particle_width(part_w), particle_height(part_h)
{
	//need one to bind, I know this is stupid
	particles.position.push_back(glm::vec3(-200, -200, -200));
	particles.velocity.push_back(glm::vec3(0, 0, 0));
	particles.frame.push_back(0);

	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(assetPath + filename);
	texture = new tdogl::Texture(bmp, GL_LINEAR, GL_REPEAT);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//verts position
	glGenBuffers(1, &pos_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.position.size() * sizeof(glm::vec3),
		&particles.position[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POS_ATTRIB);
	glVertexAttribPointer(POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	//velocity
	glGenBuffers(1, &vel_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vel_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.velocity.size() * sizeof(glm::vec3),
		&particles.velocity[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(VEL_ATTRIB);
	glVertexAttribPointer(VEL_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	//start frame
	glGenBuffers(1, &frame_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(int),
		&particles.frame[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(FRAME_ATTRIB);
	glVertexAttribIPointer(FRAME_ATTRIB, 1, GL_INT, sizeof(int), 0);

	particles.position.clear();
	particles.frame.clear();
	particles.velocity.clear();

	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	max_time = num_frames * frame_time;
	time_uniform = 0.0f;
}


Emitter::~Emitter() {
	delete texture;
}

void Emitter::update(double deltaTime) {
	time_uniform += deltaTime;
	if (time_uniform > max_time) {
		time_uniform = 0.0f;
	}
}

int Emitter::getNumParticles() {
	return particles.position.size();
}