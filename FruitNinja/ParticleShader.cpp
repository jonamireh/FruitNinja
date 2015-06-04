#include "ParticleShader.h"
#include "FlameEmitter.h"
#include "FireEmitter.h"

#define PI 3.14159

ParticleShader::ParticleShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader) {
	glBindAttribLocation(getProgramID(), POS_ATTRIB, "aPosition");
	glBindAttribLocation(getProgramID(), FRAME_ATTRIB, "aFrame");
	width_handle = getUniformHandle("uWidth");
	height_handle = getUniformHandle("uHeight");
	size_handle = getUniformHandle("uSize");
	vp_handle = getUniformHandle("uViewProjMatrix");
	eye_handle = getUniformHandle("uEyePos");
	texture_handle = getUniformHandle("Utex");
}

ParticleShader::ParticleShader(std::string vertShader, std::string geomShader, std::string fragShader) : Shader(vertShader, geomShader, fragShader) {
	glBindAttribLocation(getProgramID(), POS_ATTRIB, "aPosition");
	glBindAttribLocation(getProgramID(), FRAME_ATTRIB, "aFrame");
	width_handle = getUniformHandle("uWidth");
	height_handle = getUniformHandle("uHeight");
	size_handle = getUniformHandle("uSize");
	vp_handle = getUniformHandle("uViewProjMatrix");
	eye_handle = getUniformHandle("uEyePos");
	texture_handle = getUniformHandle("Utex");
}

void ParticleShader::draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents, std::vector<Light*> lights) {
	printf("Don't call this\n");
}

void ParticleShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity) {
	printf("Don't call this either\n");
}

void ParticleShader::draw(std::shared_ptr<Camera> camera, vector<Emitter*> emitters, std::vector<Light*> lights) {

	check_gl_error("Particle beginning of draw function");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	for (int i = 0; i < emitters.size(); i++) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, emitters[i]->texture->object());
		glUniform1i(texture_handle, 0);

		glUniform1i(width_handle, emitters[i]->atlas_width);
		glUniform1i(height_handle, emitters[i]->atlas_height);
		glUniform1f(size_handle, emitters[i]->particle_size);

		glUniformMatrix4fv(vp_handle, 1, GL_FALSE, glm::value_ptr(projection * camera->getViewMatrix()));

		glUniform3fv(eye_handle, 1, glm::value_ptr(camera->cameraPosition));

		glBindVertexArray(emitters[i]->VAO);
		FlameEmitter* flem;
		if ((flem = dynamic_cast<FlameEmitter*>(emitters[i])) != nullptr) {
			flem->update(seconds_passed, lights);
		}
		else {
			//emitters[i]->update(seconds_passed);
		}

		check_gl_error("Before particle draw");
		glDrawArrays(GL_POINTS, 0, emitters[i]->getNumParticles());
		check_gl_error("After particle draw");
	}

	glDisable(GL_BLEND);

}