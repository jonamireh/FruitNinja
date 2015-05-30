#include "Emitter.h"
#define NUM_FRAMES 36
#define FRAME_TIME 0.016

Emitter::Emitter() {
	//need one to bind, I know this is stupid
	particles.position.push_back(glm::vec3(-200, -200, -200));
	particles.frame.push_back(0);

	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "nice_fire.png");
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

void Emitter::update(double deltaTime, std::vector<Light*> lights) {
	static int prev_size = 0;
	static double prev_frame_time = 0.0;
	prev_frame_time += deltaTime;
	if (prev_frame_time > FRAME_TIME) {
		int inc = prev_frame_time / FRAME_TIME; //integer divide tells how many frames pass if any
		prev_frame_time -= inc * FRAME_TIME; //should keep remainder
		for (int i = 0; i < particles.frame.size(); i++) {
			particles.frame[i] = (particles.frame[i] + inc) % NUM_FRAMES;
		}
	}

	//if the size of the lights vector changed reset particles
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
			particles.frame.push_back(rand() % 36);
		}
		glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
		glBufferData(GL_ARRAY_BUFFER, particles.position.size() * sizeof(glm::vec3), &particles.position[0], GL_STATIC_DRAW);
		prev_size = lights.size();
	}

	//glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	//glBufferData(GL_ARRAY_BUFFER, particles.midpt.size() * sizeof(glm::vec3), &particles.midpt[0], GL_DYNAMIC_DRAW);
	
	//not really sure how to do this on the gpu so I just send it every frame...
	glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(int), &particles.frame[0], GL_DYNAMIC_DRAW);
}

int Emitter::getNumParticles() {
	return particles.position.size();
}