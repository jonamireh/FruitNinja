#pragma once

#include "Shader.h"
#include "ArcheryCamera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ArcShader : public Shader
{
public:
	ArcShader(std::string vertShader, std::string fragShader);
	void draw(ArcheryCamera* a_camera);
	GLuint VBO;
	std::vector<float> translations;
	bool draw_at_least_once = false;
};