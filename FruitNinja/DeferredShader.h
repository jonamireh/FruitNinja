#pragma once

#include "Shader.h"
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>

class DeferredShader : public Shader
{
public:
	DeferredShader(std::string vertShader, std::string fragShader);
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity);
};