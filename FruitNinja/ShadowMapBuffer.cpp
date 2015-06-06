#include <stdio.h>
#include "ShadowMapBuffer.h"

bool ShadowMapBuffer::init(unsigned int window_width, unsigned int window_height)
{
	glGenFramebuffers(1, &m_fbo);

	glGenTextures(1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadow_map_size, shadow_map_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	

	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, shadow_map_size, shadow_map_size, 0, GL_RED, GL_FLOAT, nullptr);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMapBuffer::bind_for_writing(GLenum cube_face)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cube_face, m_shadowMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void ShadowMapBuffer::bind_for_reading(GLenum texture_unit)
{
	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
}

void ShadowMapBuffer::bind_for_reading_fb(GLenum cube_face)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cube_face, m_shadowMap, 0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void ShadowMapBuffer::dump_to_screen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bind_for_reading_fb(GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	glBlitFramebuffer(0, 0, 1024, 1024,
		0, 0, 299, 299, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	bind_for_reading_fb(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	glBlitFramebuffer(0, 0, 1024, 1024,
		300, 0, 599, 299, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	bind_for_reading_fb(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	glBlitFramebuffer(0, 0, 1024, 1024,
		600, 0, 899, 299, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	bind_for_reading_fb(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	glBlitFramebuffer(0, 0, 1024, 1024,
		0, 300, 299, 599, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	bind_for_reading_fb(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	glBlitFramebuffer(0, 0, 1024, 1024,
		300, 300, 599, 599, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	bind_for_reading_fb(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	glBlitFramebuffer(0, 0, 1024, 1024,
		600, 300, 899, 599, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}