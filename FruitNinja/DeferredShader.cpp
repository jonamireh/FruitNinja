#include "World.h"
#include "DeferredShader.h"
#include "SimpleTextureShader.h"

using namespace glm;
using namespace std;



DeferredShader::DeferredShader(std::string vertShader, std::string fragShader, std::shared_ptr<Skybox> skybox)
	: Shader(vertShader, fragShader), skybox(skybox), gbuffer(), skyShader("simpleVert.glsl", "simpleFrag.glsl"),
	renderer("lightVert.glsl", "pointLightFrag.glsl", &gbuffer), disp_mode(deferred), fireShader("FireVert.glsl", "FireFrag.glsl")
{
	gbuffer.Init(SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindAttribLocation(getProgramID(), 0, "aPosition");
	glBindAttribLocation(getProgramID(), 1, "aNormal");
}

void DeferredShader::geomPass(mat4& view_mat, std::vector<std::shared_ptr<GameEntity>> ents)
{
	check_gl_error("Before geom pass");

	gbuffer.BindForGeomPass();

	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	for (int i = 0; i < ents.size(); i++) {
		std::vector<Mesh*> meshes = ents[i]->mesh->getMeshes();

		glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));
		glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(ents[i]->getModelMat()));
		glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(projection));

		for (int i = 0; i < meshes.size(); i++)
		{
			Mesh* mesh = meshes.at(i);
			glBindVertexArray(mesh->VAO);

			if (mesh->textures.size() > 0) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textures.at(0).id);
				glUniform1i(getUniformHandle("Utex"), 0);
				glUniform1i(getUniformHandle("Uflag"), 1);
			}
			else {

				glUniform1i(getUniformHandle("Uflag"), 0);
			}
			if (mesh->bones.size() > 0)
			{
				glUniform1i(getUniformHandle("uBoneFlag"), 1);
				glUniformMatrix4fv(getUniformHandle("uBones[0]"), mesh->boneTransformations.size(), GL_FALSE, value_ptr(mesh->boneTransformations[0]));
			}
			else
			{
				glUniform1i(getUniformHandle("uBoneFlag"), 0);
			}


			Material material = mesh->mat;
			glUniform3fv(getUniformHandle("UdColor"), 1, value_ptr(material.diffuse));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);

			check_gl_error("Mesh.draw before texture");

			glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

			check_gl_error("Mesh.draw after texture");

			if (mesh->textures.size() > 0) {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			
		}
	}
	glDepthMask(GL_FALSE);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}



void DeferredShader::draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents, std::vector<Light*> lights)
{
	gbuffer.StartFrame();
	geomPass(camera->getViewMatrix(), ents);

	if (disp_mode == four_screen) {
		lightPass();
	}
	else {
		//startLightPasses();
		renderer.draw(camera, ents, lights);
		
		skyboxPass(camera);
		//particlePass(camera);
		finalPass();
	}
		
	if (keys[GLFW_KEY_4])
		disp_mode = deferred;
	if (keys[GLFW_KEY_5])
		disp_mode = four_screen;
}

void DeferredShader::particlePass(std::shared_ptr<Camera> camera) {
	glUseProgram(fireShader.getProgramID());

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	fireShader.draw(camera, em);
}

void DeferredShader::skyboxPass(std::shared_ptr<Camera> camera)
{
	glUseProgram(skyShader.getProgramID());

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	skyShader.draw(camera->getViewMatrix(), skybox);
}

void DeferredShader::finalPass()
{
	getProgramID();
	gbuffer.BindForFinalPass();
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void DeferredShader::startLightPasses()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	gbuffer.BindAllForReading();
	glClear(GL_COLOR_BUFFER_BIT);
}


void DeferredShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity)
{
	std::cout << "Fuck you\n";
}

void DeferredShader::lightPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gbuffer.BindForReading();

	GLsizei HalfWidth = (GLsizei)(SCREEN_WIDTH / 2.0f);
	GLsizei HalfHeight = (GLsizei)(SCREEN_HEIGHT / 2.0f);

	gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		0, HalfHeight, HalfWidth, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		HalfWidth, HalfHeight, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	/*gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		HalfWidth, 0, SCREEN_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);*/
}