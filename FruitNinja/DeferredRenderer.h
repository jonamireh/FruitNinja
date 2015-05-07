#pragma once

#include "Shader.h"		
#include "StencilShader.h"

class DeferredRenderer : public Shader
{
public:
	DeferredRenderer(std::string vertShader, std::string fragShader, GBuffer* gbuffer);
	~DeferredRenderer();
	virtual void draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents);
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity);
	
private:
	std::vector<std::shared_ptr<Light>> lights;
	void pointLightPass(std::shared_ptr<Camera> camera, std::shared_ptr<Light> light);
	GBuffer* gbuffer;
	StencilShader stencilShader;
};