#pragma once
#include "World.h"
#include <glm/glm.hpp>
#include "tdogl/Texture.h"
#include "tdogl/Bitmap.h"

class Particle {
public:
	glm::vec3 position;
	int size;
	tdogl::Texture* texture;

	Particle(glm::vec3 pos, int s, std::string filename, GLuint texInterpolation = GL_LINEAR, GLuint texWrap = GL_REPEAT);
	Particle(glm::vec3 pos, int s, tdogl::Texture* text);
	~Particle();
};