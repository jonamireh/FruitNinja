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
	//glBindAttribLocation(getProgramID(), 1, "aNormal");

	uViewMatrixHandle = getUniformHandle("uViewMatrix");
	uModelMatrixHandle = getUniformHandle("uModelMatrix");
	uProjMatrixHandle = getUniformHandle("uProjMatrix");
	//UtexHandle = getUniformHandle("Utex");
	//UflagHandle = getUniformHandle("Uflag");
	uBoneFlagHandle = getUniformHandle("uBoneFlag");
	uBonesHandle = getUniformHandle("uBones[0]");
	//UdColorHandle = getUniformHandle("UdColor");

	vec3 cameraPoint = vec3(115.0f, 0.0f, 115.0f) - normalize(lightDirection) * 150.0f;

	view_mat = lookAt(cameraPoint, cameraPoint + lightDirection, vec3(0.0f, 0.0f, 1.0f));
	projection_mat = ortho(-200.0f, 200.0f, -200.0f, 200.0f, 1.0f, 225.0f);
	mat4 VP = projection_mat * view_mat;
}

DirShadowMapShader::~DirShadowMapShader()
{
}

void DirShadowMapShader::draw(vector<GameEntity*> ents)
{
	map<MeshSet*, vector<GameEntity*>> objectMeshes;

	for (int i = 0; i < ents.size(); i++) {
		map<MeshSet*, vector<GameEntity*>>::iterator meshItr = objectMeshes.find(ents[i]->mesh);
		objectMeshes[ents[i]->mesh].push_back(ents[i]);
	}

	glUseProgram(getProgramID());

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	shadowMapBuffer->bindForWriting();
	glViewport(0, 0, shadowMapBuffer->shadowMapSize, shadowMapBuffer->shadowMapSize);

	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uViewMatrixHandle, 1, GL_FALSE, value_ptr(view_mat));
	glUniformMatrix4fv(uProjMatrixHandle, 1, GL_FALSE, value_ptr(projection_mat));

	for (auto& currMeshSet : objectMeshes) {
		std::vector<Mesh*> meshes = currMeshSet.first->getMeshes();
		
		for (int j = 0; j < meshes.size(); j++)
		{
			Mesh* mesh = meshes[j];
			glBindVertexArray(mesh->VAO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);

			if (mesh->bones.size() > 0)
			{
				glUniform1i(uBoneFlagHandle, 1);

				map<std::vector<std::vector<glm::mat4>>*, vector<GameEntity*>> meshAnimations;

				for (auto& entity : currMeshSet.second) {
					map<std::vector<std::vector<glm::mat4>>*, vector<GameEntity*>>::iterator meshItr = meshAnimations.find(entity->getBoneTransformations());
					meshAnimations[entity->getBoneTransformations()].push_back(entity);
				}

				for (auto& currAnimation : meshAnimations) {
					std::vector<std::vector<glm::mat4>>* boneTs = currAnimation.first;
					glUniformMatrix4fv(uBonesHandle, boneTs->at(j).size(), GL_FALSE, value_ptr(boneTs->at(j)[0]));

					for (auto& entity : currAnimation.second) {
						glUniformMatrix4fv(uModelMatrixHandle, 1, GL_FALSE, value_ptr(entity->getModelMat()));

						check_gl_error("Def shader before draw");

						glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

						check_gl_error("Def shader after draw");
					}
				}
			}
			else
			{
				glUniform1i(uBoneFlagHandle, 0);

				for (auto& entity : currMeshSet.second) {
					glUniformMatrix4fv(uModelMatrixHandle, 1, GL_FALSE, value_ptr(entity->getModelMat()));

					check_gl_error("Def shader before draw");

					glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

					check_gl_error("Def shader after draw");
				}
			}
		}
	}

	glCullFace(GL_BACK);
	glDepthMask(GL_FALSE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screen_width, screen_height);
	glUseProgram(0);
}

void DirShadowMapShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity)
{
	std::cout << "I won't draw!";
}