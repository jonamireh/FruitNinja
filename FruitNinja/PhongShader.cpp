#include "PhongShader.h"
#include "World.h"

using namespace glm;
using namespace std;

PhongShader::PhongShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader)
{
	
}


void PhongShader::draw(mat4& view_mat, shared_ptr<GameEntity> entity)
{
	//vertex attributes
	int vertex = getAttributeHandle("aPosition");
	int normal = getAttributeHandle("aNormal");
	
	std::vector<Mesh *> meshes = entity->mesh->getMeshes();
	
	vec3 dir_light_vec(0, 1, 0);
	glUniform3fv(getUniformHandle("UdirLight"), 1, value_ptr(dir_light_vec));
	
	vec3 light_pos(0, 2, 2);
	glUniform3fv(getUniformHandle("uLightPos"), 1, value_ptr(light_pos));


	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));
	glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(translate(mat4(1.0f), entity->position)));
    glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 100.f)));


	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh* mesh = meshes.at(i);

		Material material = mesh->mat;
		glUniform3fv(getUniformHandle("UaColor"), 1, value_ptr(material.ambient));
		glUniform3fv(getUniformHandle("UdColor"), 1, value_ptr(material.diffuse));
		glUniform3fv(getUniformHandle("UsColor"), 1, value_ptr(material.specular));
		glUniform1f(getUniformHandle("Ushine"), material.shininess);

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