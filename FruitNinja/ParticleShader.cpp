#include "ParticleShader.h"
#include "FlameEmitter.h"

#define PI 3.14159

ParticleShader::ParticleShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader) {
	glBindAttribLocation(getProgramID(), POS_ATTRIB, "aPosition");
	glBindAttribLocation(getProgramID(), FRAME_ATTRIB, "aFrame");
}

ParticleShader::ParticleShader(std::string vertShader, std::string geomShader, std::string fragShader) : Shader(vertShader, geomShader, fragShader) {
	glBindAttribLocation(getProgramID(), POS_ATTRIB, "aPosition");
	glBindAttribLocation(getProgramID(), FRAME_ATTRIB, "aFrame");
}

void ParticleShader::draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents, std::vector<Light*> lights) {
	printf("Don't call this\n");
}

void ParticleShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity) {
	printf("Don't call this either\n");
}

void ParticleShader::draw(std::shared_ptr<Camera> camera, Emitter& emitter, std::vector<Light*> lights) {

	check_gl_error("Particle beginning of draw function");

	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, emitter.texture->object());
	glUniform1i(getUniformHandle("Utex"), 0);

	glUniformMatrix4fv(getUniformHandle("uViewProjMatrix"), 1, GL_FALSE, glm::value_ptr(projection * camera->getViewMatrix()));

	glUniform3fv(getUniformHandle("uEyePos"), 1, glm::value_ptr(camera->cameraPosition));

	glBindVertexArray(emitter.VAO);
	FlameEmitter* flem;
	if ((flem = dynamic_cast<FlameEmitter*>(&emitter)) != nullptr) {
		flem->update(seconds_passed, lights);
	}
	else {
		printf("Unrecognized emitter type\n");
	}
	
	check_gl_error("Before particle draw");
	glDrawArrays(GL_POINTS, 0, emitter.getNumParticles());
	check_gl_error("After particle draw");

	glDisable(GL_BLEND);

}