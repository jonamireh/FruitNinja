#include "Emitter.h"

Emitter::Emitter(string filename, int nframes, double tframe) : num_frames(nframes), frame_time(tframe){
	//need one to bind, I know this is stupid
	particles.position.push_back(glm::vec3(-200, -200, -200));
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
	static double prev_frame_time = 0.0;
	prev_frame_time += deltaTime;
	if (prev_frame_time > frame_time) {
		int inc = prev_frame_time / frame_time; //integer divide tells how many frames pass if any
		prev_frame_time -= inc * frame_time; //should keep remainder
		for (int i = 0; i < particles.frame.size(); i++) {
			particles.frame[i] = (particles.frame[i] + inc) % num_frames;
		}
	}
	
	//not really sure how to do this on the gpu so I just send it every frame...
	glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
	glBufferData(GL_ARRAY_BUFFER, particles.frame.size() * sizeof(int), &particles.frame[0], GL_DYNAMIC_DRAW);
}

int Emitter::getNumParticles() {
	return particles.position.size();
}