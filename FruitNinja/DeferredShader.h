#pragma once

#include "Shader.h"
#include "DeferredRenderer.h"

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
	DeferredShader(std::string vertShader, std::string fragShader);
	virtual void draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents);
	void draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity);


private:
	void geomPass(glm::mat4& view_mat, std::vector<std::shared_ptr<GameEntity>> ents);
	void lightPass();
	GBuffer gbuffer;
	void pointLightsPass();
	void dirLightPass(std::shared_ptr<Camera> camera);
	void startLightPasses();
	void finalPass();

	DeferredRenderer renderer;
	DisplayMode disp_mode;

};