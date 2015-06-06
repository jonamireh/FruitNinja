#pragma once

#include "Shader.h"
#include "DirShadowMapBuffer.h"

class DirShadowMapShader : public Shader
{
	DirShadowMapBuffer *shadowMapBuffer;
	GLint uViewMatrixHandle;
	GLint uModelMatrixHandle;
	GLint uProjMatrixHandle;
	GLint UtexHandle;
	GLint UflagHandle;
	GLint uBoneFlagHandle;
	GLint uBonesHandle;
	GLint UdColorHandle;

public:
	DirShadowMapShader(std::string vertShader, std::string fragShader, DirShadowMapBuffer *buffer, glm::vec3 lightDirection);
	~DirShadowMapShader();
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity);
	void draw(std::vector<GameEntity*> ents);

	glm::mat4 view_mat;
	glm::mat4 projection_mat;
};