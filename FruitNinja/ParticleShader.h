#pragma once

#include "Emitter.h"
#include "Shader.h"



class ParticleShader : public Shader
{
public:
	ParticleShader(std::string vertShader, std::string fragShader);

	ParticleShader(std::string vertShader, std::string geomShader, std::string fragShader);

	virtual void draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents, std::vector<Light*> lights);

	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity);

	void draw(std::shared_ptr<Camera> camera, Emitter& emitter, std::vector<Light*> lights);
};