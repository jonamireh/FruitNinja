#pragma once

#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SimpleTextureShader : public Shader
{
public:
	SimpleTextureShader(std::string vertShader, std::string fragShader);
	void draw(glm::mat4& view_mat, GameEntity* entity) override;
};