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
#include "Camera.h"
#include "Light.h"

class Shader
{
	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint geometryShader;
	std::map<std::string, GLint> attributes;
	std::map<std::string, GLint> uniforms;
	std::map<std::string, GLint> uniformBlocks;
public:
	Shader();
	Shader(std::string vertShader, std::string fragShader);
	Shader(std::string vertShader, std::string geomShader, std::string fragShader);
	void setupHandles();
	GLint getAttributeHandle(std::string name);
	GLint getUniformHandle(std::string name);
	GLint getUniformBlockHandle(std::string name);
	GLuint getProgramID();

	virtual void draw(glm::mat4& view_mat, GameEntity* entity);
	virtual void draw(Camera* camera, std::vector<GameEntity*> ents, std::vector<Light*> lights);


	bool check_gl_error(std::string msg);
};