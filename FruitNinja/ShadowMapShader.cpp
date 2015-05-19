#include "ShadowMapShader.h"
#include "ShadowMapCamera.h"
#include "World.h"

using namespace glm;

struct CameraDirection
{
	GLenum cubemap_face;
	glm::vec3 target;
	glm::vec3 up;
};

CameraDirection gCameraDirections[6] =
{
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
};

ShadowMapShader::ShadowMapShader(std::string vertShader, std::string fragShader, ShadowMapBuffer *shadowMapBuf)
	: Shader(vertShader, fragShader), shadowMapBuffer(shadowMapBuf)
{
}

void ShadowMapShader::shadowMapPass(Light* light, std::vector<std::shared_ptr<GameEntity>> ents)
{
	glUseProgram(getProgramID());

	glDepthMask(GL_TRUE);
	glCullFace(GL_FRONT);
	glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

	mat4 proj = perspective((float)radians(90.0f), 1.0f, 0.01f, 100.0f);

	for (int i = 0; i < 6; i++) {
		shadowMapBuffer->bind_for_writing(gCameraDirections[i].cubemap_face);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		ShadowMapCamera camera(light->pos, gCameraDirections[i].target, gCameraDirections[i].up);
		mat4 view_mat = camera.getViewMatrix();
		
		glUniform3f(getUniformHandle("uLightWorldPos"), light->pos.x, light->pos.x, light->pos.x);
		glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));
		glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(proj));
		
		for (int i = 0; i < ents.size(); i++) {
			std::vector<Mesh*> meshes = ents[i]->mesh->getMeshes();
			
			glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(ents[i]->getModelMat()));
			
			for (int i = 0; i < meshes.size(); i++)
			{
				Mesh* mesh = meshes.at(i);
				glBindVertexArray(mesh->VAO);
				
				if (mesh->bones.size() > 0)
				{
					glUniform1i(getUniformHandle("uBoneFlag"), 1);
					glUniformMatrix4fv(getUniformHandle("uBones[0]"), mesh->boneTransformations.size(), GL_FALSE, value_ptr(mesh->boneTransformations[0]));
				}
				else
				{
					glUniform1i(getUniformHandle("uBoneFlag"), 0);
				}
				
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);
				glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

				check_gl_error("Mesh.draw after texture");
			}
		}
	}

	glCullFace(GL_BACK);
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glClearColor(0, 0, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void ShadowMapShader::draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents)
{
	std::cout << "I won't draw either!";
}

void ShadowMapShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity)
{
	std::cout << "I won't draw!";
}