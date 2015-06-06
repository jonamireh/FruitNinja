#pragma once

#include "Shader.h"		
#include "StencilShader.h"
#include "DirLightShader.h"

class DeferredRenderer : public Shader
{
public:
	DeferredRenderer(std::string vertShader, std::string fragShader, GBuffer* gbuffer);
	~DeferredRenderer();
	virtual void draw(Camera* camera, std::vector<GameEntity*> ents, std::vector<Light*> lights);
	void draw(glm::mat4& view_mat, GameEntity* entity) override;
private:
	void pointLightPass(Camera* camera, Light* light);
	GBuffer* gbuffer;
	StencilShader stencilShader;
	DirLightShader dirLightShader;

	GLuint model_handle;
	GLuint view_handle;
	GLuint proj_handle;

	GLuint pos_map_handle;
	GLuint color_map_handle;
	GLuint normal_map_handle;

	GLuint eye_handle;
	GLuint size_handle;
	GLuint position_handle;
	GLuint light_pos_handle;
};