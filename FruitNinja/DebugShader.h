#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

class DebugShader : public Shader
{
	GLuint LINE_VAO;
	GLuint LINE_VBO;

	GLuint POINT_VAO;
	GLuint POINT_VBO;

public:
	DebugShader(std::string vertShader, std::string fragShader);
	void drawLine(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, glm::mat4& view_mat);
	void drawPoint(glm::vec3 p, glm::vec3 color, float radius, glm::mat4& view_mat);
};