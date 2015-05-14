#pragma once

#include <GL/glew.h>

class ShadowMapBuffer
{
public:
	bool init(unsigned int window_width, unsigned int window_height);
	void bind_for_writing(GLenum cube_face);
	void bind_for_reading(GLenum texture_unit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
	GLuint m_depth;
};