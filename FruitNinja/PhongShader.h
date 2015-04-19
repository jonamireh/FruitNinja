#pragma once

#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

class PhongShader : public Shader
{
public:
	PhongShader(std::string vertShader, std::string fragShader);
	void draw(std::auto_ptr<GameEntity> entity, glm::mat4 view_mat4);
};