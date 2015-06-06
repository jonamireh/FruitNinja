#pragma once

#include "Shader.h"
#include "DirShadowMapBuffer.h"

class ShadowBufferShader : public Shader
{
	DirShadowMapBuffer* shadowMapBuffer;

	static const GLfloat g_quad_vertex_buffer_data[];
	GLuint quad_vertexbuffer;
	GLuint VAO;

public:
	ShadowBufferShader(DirShadowMapBuffer* shadowMapBuffer);
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity);
	void draw();
};