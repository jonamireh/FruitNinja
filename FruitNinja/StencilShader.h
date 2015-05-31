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
	void stencilPass(std::shared_ptr<Camera> camera, GBuffer* gbuffer, Light* light);
private:
	GLuint model_handle;
	GLuint view_handle;
	GLuint proj_handle;
};