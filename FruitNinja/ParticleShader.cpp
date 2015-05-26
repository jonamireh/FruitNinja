#include "ParticleShader.h"

ParticleShader::ParticleShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader) {
	glBindAttribLocation(getProgramID(), POS_ATTRIB, "aPosition");
	glBindAttribLocation(getProgramID(), FRAME_ATTRIB, "aFrame");
}

void ParticleShader::draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents, std::vector<Light*> lights) {
	printf("Don't call this\n");
}

void ParticleShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity) {
	printf("Don't call this either\n");
}

void ParticleShader::draw(std::shared_ptr<Camera> camera, Emitter& emitter) {

	check_gl_error("Particle beginning of draw function");

	glEnable(GL_PROGRAM_POINT_SIZE);

	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	check_gl_error("Before particle Point sprite calls");
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, emitter.texture->object());
	glUniform1i(getUniformHandle("Utex"), 0);

	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(emitter.VAO);
	emitter.update(seconds_passed);
	
	check_gl_error("Before particle draw");
	glDrawArrays(GL_POINTS, 0, emitter.getNumParticles());
	check_gl_error("After particle draw");

}