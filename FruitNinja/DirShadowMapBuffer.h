#pragma once

#include <GL/glew.h>

class DirShadowMapBuffer
{
	GLuint fbo;
	GLuint shadowMap;

public:
	bool init(int windowWidth, int windowHeight);
	void bindForWriting();
	void bindForReading(GLenum textureUnit);

	const int shadowMapSize = 2048;
};