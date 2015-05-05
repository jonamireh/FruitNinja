#include "World.h"
#include "SimpleTextureShader.h"

using namespace glm;
using namespace std;

SimpleTextureShader::SimpleTextureShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader)
{
	//glBindAttribLocation(getProgramID(), 0, "aPosition");
	//glBindAttribLocation(getProgramID(), 1, "aNormal");
}


void SimpleTextureShader::draw(mat4& view_mat, shared_ptr<GameEntity> entity)
{
	//int texture = getAttributeHandle("aTextCoord");
	
	std::vector<Mesh *> meshes = entity->mesh->getMeshes();

	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));
    glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(entity->getModelMat()));
    glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 800.f)));

	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh* mesh = meshes.at(i);
		glBindVertexArray(mesh->VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textures.at(0).id);
		glUniform1i(getUniformHandle("Utex"), 0);
		//glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO3);
		//glEnableVertexAttribArray(texture);//
		//glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE,//
		//	sizeof(vec2), 0);//

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);
		
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);

		//glDisableVertexAttribArray(texture);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	
}