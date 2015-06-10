#pragma once

#include "Shader.h"
#include "DeferredRenderer.h"
#include "SimpleTextureShader.h"
#include "ParticleShader.h"
#include "FlameEmitter.h"
#include "FireEmitter.h"
#include "ArcShader.h"
#include "DirShadowMapBuffer.h"
#include "ShadowBufferShader.h"


//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>

enum DisplayMode
{
	deferred,
	four_screen
};

class DeferredShader : public Shader
{
public:
	DeferredShader(std::string vertShader, std::string fragShader, Skybox* skybox);
	~DeferredShader();
	virtual void draw(Camera* camera, std::vector<GameEntity*> ents, std::vector<Light*> lights) override;
	void draw(glm::mat4& view_mat, GameEntity* entity);
	ArcShader arcShader;


private:
	void geomPass(glm::mat4& view_mat, std::vector<GameEntity*> ents);
	void lightPass();
	GBuffer gbuffer;
	DirShadowMapBuffer dirShadowMapBuffer;
	void startLightPasses();
	void finalPass();
	void skyboxPass(Camera* camera);
	void particlePass(Camera* camera, std::vector<Light*> lights, std::vector<GameEntity*> ents);
	Skybox* skybox;
	SimpleTextureShader skyShader;
	ParticleShader fireShader;
	vector<Emitter*> emitters;

	ShadowBufferShader shadowBufferShader;
	void archeryArcPass(Camera* camera);

	DeferredRenderer renderer;
	DisplayMode disp_mode;

	GLint uViewMatrixHandle;
	GLint uModelMatrixHandle;
	GLint uProjMatrixHandle;
	GLint UtexHandle;
	GLint UflagHandle;
	GLint uBoneFlagHandle;
	GLint uBonesHandle;
	GLint UdColorHandle;
};