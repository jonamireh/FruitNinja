#include "DeferredShader.h"
#include "World.h"

using namespace glm;
using namespace std;



DeferredShader::DeferredShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader),
gbuffer(), renderer("lightVert.glsl", "pointLightFrag.glsl", &gbuffer), disp_mode(deferred)
{
	gbuffer.Init(SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindAttribLocation(getProgramID(), 0, "aPosition");
	glBindAttribLocation(getProgramID(), 1, "aNormal");
}

void DeferredShader::geomPass(mat4& view_mat, std::vector<std::shared_ptr<GameEntity>> ents)
{
	gbuffer.BindForGeomPass();

	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glEnable(GL_DEPTH_TEST);

	//glDisable(GL_BLEND);

	for (int i = 0; i < ents.size(); i++) {
		std::vector<Mesh*> meshes = ents[i]->mesh->getMeshes();

		int texture = getAttributeHandle("aTextCoord");

		glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));
		glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(ents[i]->getModelMat()));
		glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 800.f)));
		//glUniformMatrix4fv(getUniformHandle("uNormalMatrix"), 1, GL_FALSE, value_ptr(glm::inverse(glm::transpose(view_mat * ents[i]->getModelMat()))));


		for (int i = 0; i < meshes.size(); i++)
		{
			Mesh* mesh = meshes.at(i);
			glBindVertexArray(mesh->VAO);

			if (mesh->textures.size() > 0) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textures.at(0).id);
				glUniform1i(getUniformHandle("Utex"), 0);
				glUniform1i(getUniformHandle("Uflag"), 1);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO3);
				glEnableVertexAttribArray(texture);//
				glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
			}
			else {

				glUniform1i(getUniformHandle("Uflag"), 0);
			}

			Material material = mesh->mat;
			glUniform3fv(getUniformHandle("UdColor"), 1, value_ptr(material.diffuse));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);

			glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

			check_gl_error("Mesh.draw after texture");

			if (mesh->textures.size() > 0) {
				glDisableVertexAttribArray(texture);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			
		}
	}
	glDepthMask(GL_FALSE);

	//glDisable(GL_DEPTH_TEST);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}



void DeferredShader::draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents)
{
	gbuffer.StartFrame();
	geomPass(camera->getViewMatrix(), ents);

	if (disp_mode == four_screen) {
		lightPass();
	}
	else {
		//startLightPasses();
		renderer.draw(camera, ents);
		finalPass();
	}
		
	if (keys[GLFW_KEY_4])
		disp_mode = deferred;
	if (keys[GLFW_KEY_5])
		disp_mode = four_screen;
}

void DeferredShader::finalPass()
{
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