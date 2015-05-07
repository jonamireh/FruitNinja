#include "StencilShader.h"
#include "World.h"

StencilShader::StencilShader(std::string vertShader, std::string fragShader)
	: Shader(vertShader, fragShader)
{
	glBindAttribLocation(getProgramID(), 0, "aPosition");
}

void StencilShader::stencilPass(std::shared_ptr<Camera> camera, GBuffer* gbuffer, std::shared_ptr<Light> light)
{
	glUseProgram(getProgramID());

	// Disable color/depth write and enable stencil
	gbuffer->BindForStencilPass();

	glBindVertexArray(light->VAO());

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);

	// We need the stencil test to be enabled but we want it
	// to succeed always. Only the depth test matters.
	glStencilFunc(GL_ALWAYS, 0, 0);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(light->transform()));
	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, glm::value_ptr(
		glm::mat4(glm::perspective((float)glm::radians(45.0), screen_width / screen_height, 0.1f, 800.f))));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light->IND());
	check_gl_error("stencil before");
	glDrawElements(GL_TRIANGLES, light->indices_size(), GL_UNSIGNED_INT, 0);
	check_gl_error("stencil after");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void StencilShader::draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents)
{
	std::cout << "I won't draw either!";
}

void StencilShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity)
{
	std::cout << "I won't draw!";
}