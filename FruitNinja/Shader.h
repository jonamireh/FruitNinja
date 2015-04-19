#pragma once

#include <cassert>
#include <iostream>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "GLSL.h"
#include <memory>
#include "GameEntity.h"
#include <glm/glm.hpp>

class Shader
{
	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
	std::map<std::string, GLint> attributes;
	std::map<std::string, GLint> uniforms;

public:
	Shader();
	Shader(std::string vertShader, std::string fragShader);
	void setupHandles();
	GLint getAttributeHandle(std::string name);
	GLint getUniformHandle(std::string name);
	GLuint getProgramID();
	virtual void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity) =0;
};