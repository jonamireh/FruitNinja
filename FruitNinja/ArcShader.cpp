#include "ArcShader.h"
#include "World.h"

using namespace glm;
using namespace std;

#define NUM_INSTANCES 1

ArcShader::ArcShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader)
{
	//glBindAttribLocation(getProgramID(), 0, "aPosition");
	//glBindAttribLocation(getProgramID(), 1, "aNormal");
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * sizeof(mat4), glm::value_ptr(mat4(1.0)), GL_DYNAMIC_DRAW);
}


void ArcShader::draw(shared_ptr<ArcheryCamera> a_camera)
{
	int currentId;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentId);
	assert(currentId == getProgramID());
	assert(translations.size() == 0);

	glBindVertexArray(a_camera->particle->VAO);

	glBindAttribLocation(getProgramID(), 0, "aPosition");
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_camera->particle->IND);

	/*glm::vec3 left = glm::cross(a_camera->cameraFront, vec3(0.f, 1.f, 0.f));
	left.y = 0.f;*/

	for (int i = 0; i < NUM_INSTANCES; i++)
	{
		translations.push_back(glm::translate(mat4(1.f), vec3(a_camera->cameraPosition + 2.f * glm::normalize(a_camera->cameraFront))));
		// +((float)i) * glm::normalize(left));
	}

	glBindAttribLocation(getProgramID(), 1, "aTransMat");
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_INSTANCES * sizeof(mat4), glm::value_ptr(translations[0]));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(0));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 4));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 8));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 12));
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	//check_gl_error("ArcShader.draw before actual draw");

	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(a_camera->getViewMatrix()));
	glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(projection));

	mat4 comp = glm::scale(vec3(0.05f, 0.05f, 0.05f));

	glUniformMatrix4fv(getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(comp));
	glDrawElementsInstanced(GL_TRIANGLES, a_camera->particle->indices.size(), GL_UNSIGNED_INT, 0, NUM_INSTANCES);
	
	translations.clear();
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}