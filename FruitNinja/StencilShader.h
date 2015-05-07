#pragma once

#include "Shader.h"
#include "GBuffer.h"
#include "Light.h"

class StencilShader : public Shader
{
public:
	StencilShader(std::string vertShader, std::string fragShader);
	virtual void draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents);
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity);
	void stencilPass(std::shared_ptr<Camera> camera, GBuffer* gbuffer, std::shared_ptr<Light> light);
};