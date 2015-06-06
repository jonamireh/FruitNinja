#include "ShadowBufferShader.h"

const GLfloat ShadowBufferShader::g_quad_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
};

ShadowBufferShader::ShadowBufferShader(DirShadowMapBuffer* shadowMapBuffer)
	: Shader("ShadowBufferVertShader.glsl", "ShadowBufferFragShader.glsl"), shadowMapBuffer(shadowMapBuffer)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void ShadowBufferShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity)
{
	std::cout << "I won't draw!";
}
void ShadowBufferShader::draw()
{
	glUseProgram(getProgramID());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Bind our texture in Texture Unit 0
	shadowMapBuffer->bindForReading(GL_TEXTURE0);
	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(getUniformHandle("uTexture"), 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(getAttributeHandle("vertexPosition_modelspace"));
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		getAttributeHandle("vertexPosition_modelspace"), // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// Draw the triangles !
	// You have to disable GL_COMPARE_R_TO_TEXTURE above in order to see anything !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(getAttributeHandle("vertexPosition_modelspace"));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}