#include "ArcShader.h"
#include "World.h"
#include "ProjectileEntity.h"

using namespace glm;
using namespace std;

#define NUM_INSTANCES 200

ArcShader::ArcShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader)
{
	//glBindAttribLocation(getProgramID(), 0, "aPosition");
	//glBindAttribLocation(getProgramID(), 1, "aNormal");
}


void ArcShader::draw(ArcheryCamera* a_camera)
{
	int currentId;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentId);
	assert(currentId == getProgramID());
	assert(translations.size() == 0);

	glBindVertexArray(a_camera->particle->VAO);
	if (!draw_at_least_once)
	{
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 4 * sizeof(vec4), glm::value_ptr(vec3(1.0)), GL_DYNAMIC_DRAW);
		draw_at_least_once = true;
	}

	glBindAttribLocation(getProgramID(), 0, "aPosition");
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_camera->particle->IND);

	vec3 velocity = a_camera->cameraFront * ARROW_SPEED * bow_strength;
	vec3 base_transformation = a_camera->cameraPosition + 2.f * a_camera->cameraFront;
	vec3 left = glm::normalize(glm::cross(vec3(0.f, 1.f, 0.f), a_camera->cameraFront));
	vec3 offset = 0.2f * left;
	vec3 incremental_offset = offset / (float)NUM_INSTANCES;
	base_transformation += offset;
	float t1 = -(sqrt(pow(velocity.y, 2) - 2.f * -GRAVITY * base_transformation.y) + velocity.y) / -GRAVITY;
	float t2 = (sqrt(pow(velocity.y, 2) - 2.f * -GRAVITY * base_transformation.y) - velocity.y) / -GRAVITY;
	float intersection_time = glm::max(t1, t2);
	float pseudo_time = intersection_time / NUM_INSTANCES;


	for (int i = 0; i < NUM_INSTANCES; i++)
	{
		vec3 final_translate = base_transformation;
		vec3 displacement;
		float instance_time = i * pseudo_time;
		displacement.x = velocity.x * instance_time;
		displacement.y = (-0.5f * GRAVITY * pow(instance_time, 2)) + velocity.y * instance_time;
		displacement.z = velocity.z * instance_time;
		final_translate += displacement;
		final_translate -= incremental_offset * (float) i;
		translations.push_back(glm::translate(mat4(1.f), final_translate));
	}

	

	glBindAttribLocation(getProgramID(), 1, "aTransMat");
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_INSTANCES * 4 * sizeof(vec4), glm::value_ptr(translations[0]));
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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}