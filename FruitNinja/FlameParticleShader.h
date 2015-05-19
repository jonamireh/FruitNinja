#pragma once
#include "Shader.h"
#include "Flame.h"

class FlameParticleShader : public Shader
{
public:
	FlameParticleShader(std::string vertShader, std::string fragShader);
	void draw(glm::mat4& view_mat, std::shared_ptr<Flame> flame);

private:
	void init();
};