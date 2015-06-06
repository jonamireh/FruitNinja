#pragma once

#include "Shader.h"
#include "GBuffer.h"
#include "Light.h"

class StencilShader : public Shader
{
public:
	StencilShader(std::string vertShader, std::string fragShader);
	virtual void draw(Camera* camera, std::vector<GameEntity*> ents);
	void draw(glm::mat4& view_mat, GameEntity* entity);
	void stencilPass(Camera* camera, GBuffer* gbuffer, Light* light);
private:
	GLuint model_handle;
	GLuint view_handle;
	GLuint proj_handle;
};