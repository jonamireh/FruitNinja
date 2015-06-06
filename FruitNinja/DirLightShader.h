#pragma once

#include "Shader.h"		
#include "StencilShader.h"

class DirLightShader : public Shader
{
public:
	DirLightShader(GBuffer* gbuffer);
	~DirLightShader();
	virtual void draw(Camera* camera, std::vector<GameEntity*> ents, std::vector<Light*> lights) override;
	void draw(glm::mat4& view_mat, GameEntity* entity) override;
	void pass(Camera* camera);

private:
	GBuffer* gbuffer;
	GLuint VAO;
	GLuint VBO;
	GLuint IND;

	GLuint model_handle;
	GLuint view_handle;
	GLuint proj_handle;

	GLuint pos_map_handle;
	GLuint color_map_handle;
	GLuint normal_map_handle;

	GLuint eye_handle;
	GLuint size_handle;
};