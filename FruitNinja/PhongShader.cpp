#include "PhongShader.h"
#include "World.h"

using namespace glm;
using namespace std;

PhongShader::PhongShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader)
{
	//glBindAttribLocation(getProgramID(), 0, "aPosition");
	//glBindAttribLocation(getProgramID(), 1, "aNormal");
}


void PhongShader::draw(mat4& view_mat, GameEntity* entity)
{
	//int texture = getAttributeHandle("aTextCoord");
	
	std::vector<Mesh *> meshes = entity->mesh->getMeshes();
	
	vec3 dir_light_vec(0, 1, 0);
	glUniform3fv(getUniformHandle("UdirLight"), 1, value_ptr(dir_light_vec));
	
	vec3 light_pos(0, 2, 2);
	glUniform3fv(getUniformHandle("uLightPos"), 1, value_ptr(light_pos));

	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));

    glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(entity->getModelMat()));

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
			glUniformMatrix4fv(getUniformHandle("uBones[0]"), mesh->getBoneTransformations().size(), GL_FALSE, value_ptr(mesh->getBoneTransformations()[0]));
		}
		else
		{
			glUniform1i(getUniformHandle("uBoneFlag"), 0);
		}

		Material material = mesh->mat;
		glUniform3fv(getUniformHandle("UaColor"), 1, value_ptr(material.ambient));
		glUniform3fv(getUniformHandle("UdColor"), 1, value_ptr(material.diffuse));
		glUniform3fv(getUniformHandle("UsColor"), 1, value_ptr(material.specular));
		glUniform1f(getUniformHandle("Ushine"), material.shininess);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);

		check_gl_error("Mesh.draw before drawelem");
		
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		
		check_gl_error("Mesh.draw after texture");

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	
}