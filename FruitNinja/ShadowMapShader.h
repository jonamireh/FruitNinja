#pragma once

#include "Shader.h"
#include "ShadowMapBuffer.h"

class ShadowMapShader : public Shader
{
public:
	ShadowMapShader(std::string vertShader, std::string fragShader);
	virtual void draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents);
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity);
	void shadowMapPass(Light* light, std::vector<std::shared_ptr<GameEntity>> ents);
private:
	ShadowMapBuffer shadowMapBuffer;
};