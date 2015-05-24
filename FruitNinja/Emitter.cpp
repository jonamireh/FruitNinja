#include "Emitter.h"

Emitter::Emitter() {
	std::vector<glm::vec3> verts;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			particles.positions.push_back(glm::vec3((float)i, 2.0f, (float)j));
			particles.frame.push_back(0);
		}
	}

	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "nice_fire.png");
	texture = new tdogl::Texture(bmp, GL_LINEAR, GL_REPEAT);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//verts position
	glGenBuffers(1, &pos_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.positions.size() * sizeof(glm::vec3),
		&particles.positions[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(POS_ATTRIB);
	glVertexAttribPointer(POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	//bind more here
	glGenBuffers(1, &frame_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(int),
		&particles.frame[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(FRAME_ATTRIB);
	glVertexAttribIPointer(FRAME_ATTRIB, 1, GL_INT, sizeof(int), 0);

}

Emitter::~Emitter() {
	delete texture;
}

void Emitter::update(double deltaTime) {
	for (int i = 0; i < particles.positions.size(); i++) {
		//particles.positions[i].y += 1.0 * deltaTime;
		particles.frame[i]++;
		if (particles.frame[i] >= 36) {
			particles.frame[i] = 0;
			particles.positions[i].y = 2.0;
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.positions.size() * sizeof(glm::vec3), &particles.positions[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(int), &particles.frame[0], GL_DYNAMIC_DRAW);
}

int Emitter::getNumParticles() {
	return particles.positions.size();
}