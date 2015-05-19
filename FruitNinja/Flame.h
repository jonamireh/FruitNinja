#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "GLSL.h"
#include <vector>

#define NUM_PARTICLES 100000
#define TURNING_POINT (8 * M_PI) / 9

class Flame
{
public:
	Flame(glm::vec3 origin, float height, float max_radius);
	GLuint VAO;
	GLuint pVBO;
	GLuint cVBO;
	std::vector<float> pos;
	std::vector<float> col;

private:
	float height;
	float max_radius;
	glm::vec3 origin;

	void init();
	void update();

	float random_float(float a, float b) {
		float random = ((float) rand()) / (float) RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}
};