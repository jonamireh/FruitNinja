#include "World.h"
#include "DeferredRenderer.h"

using namespace glm;

DeferredRenderer::DeferredRenderer(std::string vertShader, std::string fragShader, GBuffer* gbuffer, DirShadowMapBuffer* dirShadowMapBuf)
	: Shader(vertShader, fragShader), gbuffer(gbuffer), stencilShader("StencilVert.glsl", "StencilFrag.glsl"),
	lightDir(vec3(0.2f, -1.0f, 0.2f)), dirShadowMapBuffer(dirShadowMapBuf),
	dirShadowMapShader("DeferredVertShader.glsl", "DeferredFragShader.glsl", dirShadowMapBuffer, lightDir),
	dirLightShader(gbuffer, dirShadowMapBuffer, lightDir, glm::mat4(dirShadowMapShader.projection_mat * dirShadowMapShader.view_mat))
	{
	glBindAttribLocation(getProgramID(), 0, "aPosition");

	model_handle = getUniformHandle("uModelMatrix");
	view_handle = getUniformHandle("uViewMatrix");
	proj_handle = getUniformHandle("uProjMatrix");
	pos_map_handle = getUniformHandle("posMap");
	color_map_handle = getUniformHandle("colMap");
	normal_map_handle = getUniformHandle("norMap");
	eye_handle = getUniformHandle("uEye");
	size_handle = getUniformHandle("uSize");
	position_handle = getAttributeHandle("aPosition");
	light_pos_handle = getUniformHandle("uLightPos");
	light_color_handle = getUniformHandle("uLightColor");
}

//failed attempt
DeferredRenderer::~DeferredRenderer()
{
	
}

void DeferredRenderer::pointLightPass(Camera* camera, Light* light)
{
	gbuffer->BindForLightPass();

	glUseProgram(getProgramID());

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBindVertexArray(light->VAO());

	glUniform1i(pos_map_handle, GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glUniform1i(color_map_handle, GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glUniform1i(normal_map_handle, GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);

	vec3 eye = camera->cameraPosition;
	vec3 light_pos = light->pos;
	vec3 light_color = light->color;
	glUniform3f(eye_handle, eye.x, eye.y, eye.z);
	glUniform3f(light_pos_handle, light_pos.x, light_pos.y, light_pos.z);
	glUniform3f(light_color_handle, light_color.x, light_color.y, light_color.z);
	glUniform2f(size_handle, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	glUniformMatrix4fv(model_handle, 1, GL_FALSE, value_ptr(light->transform()));
	glUniformMatrix4fv(view_handle, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(proj_handle, 1, GL_FALSE, glm::value_ptr(projection));


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light->IND());
	check_gl_error("rend before");
	glDrawElements(GL_TRIANGLES, light->indices_size(), GL_UNSIGNED_INT, 0);
	check_gl_error("rend after");


	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}


void DeferredRenderer::draw(Camera* camera, std::vector<GameEntity*> ents, std::vector<Light*> lights)
{
	glEnable(GL_STENCIL_TEST);
	for (int i = 0; i < lights.size(); i++) {
		stencilShader.stencilPass(camera, gbuffer, lights[i]);
		pointLightPass(camera, lights[i]);
	}
	glDisable(GL_STENCIL_TEST);

	dirShadowMapShader.draw(ents);
	dirLightShader.pass(camera);
}
void DeferredRenderer::draw(glm::mat4& view_mat, GameEntity* entity)
{
	std::cout << "I won't draw!";
}