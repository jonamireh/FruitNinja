#pragma once

#include "Shader.h"
#include "ArcheryCamera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ArcShader : public Shader
{
public:
	ArcShader(std::string vertShader, std::string fragShader);
	void draw(std::shared_ptr<ArcheryCamera> a_camera);
	GLuint VBO;
	std::vector<glm::mat4> translations;
};