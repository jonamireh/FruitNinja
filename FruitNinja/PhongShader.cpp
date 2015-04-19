#include "PhongShader.h"

using namespace glm;

PhongShader::PhongShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader)
{
	
}


void PhongShader::draw(std::auto_ptr<GameEntity> entity, mat4 view_mat)
{
	//vertex attributes
	int vertex = getAttributeHandle("aPosition");
	int normal = getAttributeHandle("aNormal");
	
	std::vector<Mesh *> meshes = entity->mesh.getMeshes();
	Material material = entity->material;
	glUniform3fv(getUniformHandle("UaColor"), 1, value_ptr(material.ambient));
	glUniform3fv(getUniformHandle("UdColor"), 1, value_ptr(material.diffuse));
	glUniform3fv(getUniformHandle("UsColor"), 1, value_ptr(material.specular));
	glUniform1f(getUniformHandle("Ushine"), material.shininess);

	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));
	glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(mat4(1.0f)));

	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh* mesh = meshes.at(i);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);

		//checkError("Mesh.draw");

		glEnableVertexAttribArray(vertex);
		glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);

		glEnableVertexAttribArray(normal);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexData), (void*)(sizeof(glm::vec3)));

		//checkError("Mesh.draw");

		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);


		glDisableVertexAttribArray(vertex);
		glDisableVertexAttribArray(normal);
	}

	
}