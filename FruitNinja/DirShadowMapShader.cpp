#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DirShadowMapShader.h"
#include "World.h"
#include "FrustrumCulling.h"

using namespace std;
using namespace glm;

DirShadowMapShader::DirShadowMapShader(std::string vertShader, std::string fragShader, DirShadowMapBuffer *buffer, vec3 lightDirection)
	: Shader(vertShader, fragShader), shadowMapBuffer(buffer)
{
	glBindAttribLocation(getProgramID(), 0, "aPosition");
	glBindAttribLocation(getProgramID(), 1, "aNormal");

	uViewMatrixHandle = getUniformHandle("uViewMatrix");
	uModelMatrixHandle = getUniformHandle("uModelMatrix");
	uProjMatrixHandle = getUniformHandle("uProjMatrix");
	//UtexHandle = getUniformHandle("Utex");
	//UflagHandle = getUniformHandle("Uflag");
	uBoneFlagHandle = getUniformHandle("uBoneFlag");
	uBonesHandle = getUniformHandle("uBones[0]");
	//UdColorHandle = getUniformHandle("UdColor");

	vec3 cameraPoint = vec3(115.0f, 0.0f, 115.0f) - normalize(lightDirection) * 100.0f;

	view_mat = lookAt(cameraPoint, cameraPoint + lightDirection, vec3(0.0f, 0.0f, 1.0f));
	projection_mat = ortho(-200.0f, 200.0f, -200.0f, 200.0f, 1.0f, 175.0f);
	mat4 VP = projection_mat * view_mat;
}

DirShadowMapShader::~DirShadowMapShader()
{
}

void DirShadowMapShader::draw(vector<GameEntity*> ents)
{
	std::vector<GameEntity*> entsInView = ents;//get_objects_in_view(ents, view_mat);
	glUseProgram(getProgramID());

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	shadowMapBuffer->bindForWriting();
	glViewport(0, 0, shadowMapBuffer->shadowMapSize, shadowMapBuffer->shadowMapSize);

	glClear(GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < entsInView.size(); i++) {
		std::vector<Mesh*> meshes = entsInView[i]->mesh->getMeshes();
		std::vector<std::vector<glm::mat4>>* boneTs = ents[i]->getBoneTrans();

		glUniformMatrix4fv(uViewMatrixHandle, 1, GL_FALSE, value_ptr(view_mat));
		glUniformMatrix4fv(uModelMatrixHandle, 1, GL_FALSE, value_ptr(entsInView[i]->getModelMat()));
		glUniformMatrix4fv(uProjMatrixHandle, 1, GL_FALSE, value_ptr(projection_mat));

		for (int j = 0; j < meshes.size(); j++)
		{
			Mesh* mesh = meshes[j];
			glBindVertexArray(mesh->VAO);

			/*if (mesh->textures.size() > 0) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textures.at(0).id);
				glUniform1i(UtexHandle, 0);
				glUniform1i(UflagHandle, 1);
			}
			else {

				glUniform1i(UflagHandle, 0);
			}*/

			if (mesh->bones.size() > 0)
			{
				glUniform1i(uBoneFlagHandle, 1);
				glUniformMatrix4fv(uBonesHandle, (*boneTs)[j].size(), GL_FALSE, value_ptr((*boneTs)[j][0]));
			}
			else
			{
				glUniform1i(uBoneFlagHandle, 0);
			}

			//Material material = mesh->mat;
			//glUniform3fv(UdColorHandle, 1, value_ptr(material.diffuse));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);

			//check_gl_error("Mesh.draw before texture");

			glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

			//check_gl_error("Mesh.draw after texture");

			if (mesh->textures.size() > 0) {
				glBindTexture(GL_TEXTURE_2D, 0);
			}

		}
	}

	glCullFace(GL_BACK);
	glDepthMask(GL_FALSE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glUseProgram(0);
}

void DirShadowMapShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity)
{
	std::cout << "I won't draw!";
}