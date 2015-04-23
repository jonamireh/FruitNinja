#include "PhongShader.h"
#include "World.h"

using namespace glm;
using namespace std;

bool check_gl_error(std::string msg) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << msg << ": OpenGL Error: " << error << std::endl;
		return true;
	}
	return false;
}

PhongShader::PhongShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader)
{
	
}


void PhongShader::draw(mat4& view_mat, shared_ptr<GameEntity> entity)
{
	//vertex attributes
	int vertex = getAttributeHandle("aPosition");
	int normal = getAttributeHandle("aNormal");
	//int texture = getAttributeHandle("aTextCoord");
	
	std::vector<Mesh *> meshes = entity->mesh->getMeshes();
	
	vec3 dir_light_vec(0, 1, 0);
	glUniform3fv(getUniformHandle("UdirLight"), 1, value_ptr(dir_light_vec));
	
	vec3 light_pos(0, 2, 2);
	glUniform3fv(getUniformHandle("uLightPos"), 1, value_ptr(light_pos));


	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));

    mat4 model_trans = translate(mat4(1.0f), entity->position);
    mat4 model_rot_x = rotate(mat4(1.0f), entity->rotations.x, vec3(1.f, 0.f, 0.f));
    mat4 model_rot_y = rotate(mat4(1.0f), entity->rotations.y, vec3(0.f, 1.f, 0.f));
    mat4 model_rot_z = rotate(mat4(1.0f), entity->rotations.z, vec3(0.f, 0.f, 1.f));
    mat4 model_scale = scale(mat4(1.0f), vec3(entity->scale, entity->scale, entity->scale));

    glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(model_trans * model_rot_z * model_rot_x * model_rot_y * model_scale));
    glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 800.f)));


	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh* mesh = meshes.at(i);
		if (mesh->textures.size() > 0) {
			//std::cout << "textur" << std::endl;
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, mesh->textures.at(0).id);
			//glUniform1i(getUniformHandle("Utex"), 0);
			/*glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_TEXTURE_2D);

			glBegin(GL_QUADS);
			glNormal3f(0.0, 0.0, 1.0);
			glTexCoord2d(1, 1); glVertex3f(0.0, 0.0, 0.0);
			glTexCoord2d(1, 0); glVertex3f(0.0, 1.0, 0.0);
			glTexCoord2d(0, 0); glVertex3f(1.0, 1.0, 0.0);
			glTexCoord2d(0, 1); glVertex3f(1.0, 0.0, 0.0);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			glPopAttrib();*/
		}

		Material material = mesh->mat;
		glUniform3fv(getUniformHandle("UaColor"), 1, value_ptr(material.ambient));
		glUniform3fv(getUniformHandle("UdColor"), 1, value_ptr(material.diffuse));
		glUniform3fv(getUniformHandle("UsColor"), 1, value_ptr(material.specular));
		glUniform1f(getUniformHandle("Ushine"), material.shininess);

		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IND);

		glEnableVertexAttribArray(vertex);
		glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);

		glEnableVertexAttribArray(normal);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE,
			sizeof(VertexData), (void*)(sizeof(glm::vec3)));

		check_gl_error("Mesh.draw before texture");

		//glEnableVertexAttribArray(texture);
		//glVertexAttribPointer(normal, 2, GL_FLOAT, GL_FALSE,
		//	sizeof(VertexData), (void*)(3 * sizeof(glm::vec3)));

		check_gl_error("Mesh.draw after texture");

		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);


		glDisableVertexAttribArray(vertex);
		glDisableVertexAttribArray(normal);
		//glDisableVertexAttribArray(texture);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	
}