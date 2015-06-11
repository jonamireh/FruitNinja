#pragma once

#include "Emitter.h"
#include "Shader.h"
#include "FireArrowEntity.h"



class ParticleShader : public Shader
{
public:
	ParticleShader(std::string vertShader, std::string fragShader);

	ParticleShader(std::string vertShader, std::string geomShader, std::string fragShader);

	virtual void draw(Camera* camera, std::vector<GameEntity*> ents, std::vector<Light*> lights) override;

	void draw(glm::mat4& view_mat, GameEntity* entity) override;

	void draw(Camera* camera, std::vector<Emitter*> emitters, std::vector<Light*> lights,
		std::vector<FireArrowEntity*> fireArrows, bool do_stuff_for_jon = false);

private:
	GLuint col_handle;
	GLuint row_handle;
	GLuint particle_width_handle;
	GLuint particle_height_handle;
	GLuint vp_handle;
	GLuint eye_handle;
	GLuint texture_handle;
	GLuint time_handle;
	GLuint frame_time_handle;
	GLuint accel_handle;
};