#pragma once
#include "Shader.h"
#include <glm\glm.hpp>

class TextureDebugShader :
	public Shader
{
	static const GLfloat g_quad_vertex_buffer_data[];
	GLuint quad_vertexbuffer;
public:
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity) override;
	void drawTexture(GLuint textureId, int xloc, int yloc, int width, int height);
	void drawTexture(GLuint textureId, int width, int height);
	TextureDebugShader();
	~TextureDebugShader();
};
