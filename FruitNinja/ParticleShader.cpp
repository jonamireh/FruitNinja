#include "ParticleShader.h"
#include "FlameEmitter.h"
#include "FireEmitter.h"

#define PI 3.14159

ParticleShader::ParticleShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader) {
	glBindAttribLocation(getProgramID(), POS_ATTRIB, "aPosition");
	glBindAttribLocation(getProgramID(), VEL_ATTRIB, "aVelocity");
	glBindAttribLocation(getProgramID(), FRAME_ATTRIB, "aFrame");
	col_handle = getUniformHandle("uColumns");
	row_handle = getUniformHandle("uRows");
	particle_width_handle = getUniformHandle("uPartWidth");
	particle_height_handle = getUniformHandle("uPartHeight");
	vp_handle = getUniformHandle("uViewProjMatrix");
	eye_handle = getUniformHandle("uEyePos");
	texture_handle = getUniformHandle("Utex");
	time_handle = getUniformHandle("uTime");
	frame_time_handle = getUniformHandle("uTimePerFrame");
	accel_handle = getUniformHandle("uAcceleration");
}

ParticleShader::ParticleShader(std::string vertShader, std::string geomShader, std::string fragShader)
	: Shader(vertShader, geomShader, fragShader)
{
	glBindAttribLocation(getProgramID(), POS_ATTRIB, "aPosition");
	glBindAttribLocation(getProgramID(), VEL_ATTRIB, "aVelocity");
	glBindAttribLocation(getProgramID(), FRAME_ATTRIB, "aFrame");
	col_handle = getUniformHandle("uColumns");
	row_handle = getUniformHandle("uRows");
	particle_width_handle = getUniformHandle("uPartWidth");
	particle_height_handle = getUniformHandle("uPartHeight");
	vp_handle = getUniformHandle("uViewProjMatrix");
	eye_handle = getUniformHandle("uEyePos");
	texture_handle = getUniformHandle("Utex");
	time_handle = getUniformHandle("uTime");
	frame_time_handle = getUniformHandle("uTimePerFrame");
	accel_handle = getUniformHandle("uAcceleration");
}

void ParticleShader::draw(Camera* camera, std::vector<GameEntity*> ents, std::vector<Light*> lights) {
	printf("Don't call this\n");
}

void ParticleShader::draw(glm::mat4& view_mat, GameEntity* entity) {
	printf("Don't call this either\n");
}

void ParticleShader::draw(Camera* camera, vector<Emitter*> emitters, std::vector<Light*> lights, std::vector<FireArrowEntity*> fireArrows) {

	check_gl_error("Particle beginning of draw function");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	for (int i = 0; i < emitters.size(); i++) {
		FlameEmitter* flem;
		FireEmitter* fe;
		if ((flem = dynamic_cast<FlameEmitter*>(emitters[i])) != nullptr) {
			flem->update(seconds_passed, lights);
		}
		else if ((fe = dynamic_cast<FireEmitter*>(emitters[i])) != nullptr){
			fe->update(seconds_passed, fireArrows);
		}
		else {
			emitters[i]->update(seconds_passed);
		}
		if (emitters[i]->getNumParticles() > 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, emitters[i]->texture->object());
			glUniform1i(texture_handle, 0);

			glUniform1i(col_handle, emitters[i]->columns);
			glUniform1i(row_handle, emitters[i]->rows);
			glUniform1f(particle_width_handle, emitters[i]->particle_width);
			glUniform1f(particle_height_handle, emitters[i]->particle_height);
			glUniform1f(time_handle, emitters[i]->time_uniform);
			glUniform1f(frame_time_handle, emitters[i]->frame_time);
			glUniform3fv(accel_handle, 1, glm::value_ptr(emitters[i]->acceleration));

			glUniformMatrix4fv(vp_handle, 1, GL_FALSE, glm::value_ptr(projection * camera->getViewMatrix()));

			glUniform3fv(eye_handle, 1, glm::value_ptr(camera->cameraPosition));

			glBindVertexArray(emitters[i]->VAO);


			check_gl_error("Before particle draw");
			glDrawArrays(GL_POINTS, 0, emitters[i]->getNumParticles());
			check_gl_error("After particle draw");
		}
	}

	glDisable(GL_BLEND);

}