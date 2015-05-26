#include "Particle.h"

Particle::Particle(glm::vec3 pos, int s, std::string filename, GLuint texInterpolation, GLuint texWrap) : position(pos), size(s) {

	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(assetPath + filename);
	texture = new tdogl::Texture(bmp, texInterpolation, texWrap);
}

Particle::Particle(glm::vec3 pos, int s, tdogl::Texture* tex) : position(pos), size(s), texture(tex) {
}

Particle::~Particle() {
	delete texture;
}