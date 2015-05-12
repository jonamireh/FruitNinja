#pragma once

#include "Shader.h"		
#include "StencilShader.h"

class DirLightShader : public Shader
{
public:
	DirLightShader(GBuffer* gbuffer);
	~DirLightShader();
	virtual void draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents, std::vector<Light*> lights) override;
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity) override;
	void pass(std::shared_ptr<Camera> camera);

private:
	GBuffer* gbuffer;
	GLuint VAO;
	GLuint VBO;
	GLuint IND;
};