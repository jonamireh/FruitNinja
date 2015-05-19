#pragma once

#include <GL/glew.h>

class ShadowMapBuffer
{
public:
	bool init(unsigned int window_width, unsigned int window_height);
	void bind_for_writing(GLenum cube_face);
	void bind_for_reading(GLenum texture_unit);
	void bind_for_reading_fb(GLenum cube_face);
	void dump_to_screen();

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
	GLuint m_depth;
};