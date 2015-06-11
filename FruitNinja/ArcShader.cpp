#include "ArcShader.h"
#include "World.h"
#include "ProjectileEntity.h"
#include <glm/gtc/matrix_access.hpp>

using namespace glm;
using namespace std;

#define NUM_INSTANCES 1000

ArcShader::ArcShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader)
{
	//glBindAttribLocation(getProgramID(), 0, "aPosition");
	//glBindAttribLocation(getProgramID(), 1, "aNormal");
}


void ArcShader::draw(ArcheryCamera* a_camera)
{
	if (enabled)
	{
		if (DEBUG_MODE) {
			int currentId;
			glGetIntegerv(GL_CURRENT_PROGRAM, &currentId);
			assert(currentId == getProgramID());
		}
		assert(translations.size() == 0);

		glBindVertexArray(a_camera->particle->VAO);
		if (!draw_at_least_once)
		{
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			assert(sizeof(glm::vec4) == sizeof(GLfloat) * 4, "Platform doesn't support this directly.");

			glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 4 * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
			draw_at_least_once = true;
		}

		glBindAttribLocation(getProgramID(), 0, "aPosition");
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_camera->particle->IND);

		vec3 velocity = a_camera->cameraFront * ARROW_SPEED * bow_strength;
		vec3 base_transformation = a_camera->cameraPosition + 6.f * a_camera->cameraFront;

		float t1 = -(sqrt(pow(velocity.y, 2) - 2.f * -GRAVITY * base_transformation.y) + velocity.y) / -GRAVITY;
		float t2 = (sqrt(pow(velocity.y, 2) - 2.f * -GRAVITY * base_transformation.y) - velocity.y) / -GRAVITY;
		float intersection_time = glm::max(t1, t2);
		float pseudo_time = intersection_time / NUM_INSTANCES;
		vec3 left = glm::normalize(glm::cross(vec3(0.f, 1.f, 0.f), a_camera->cameraFront));
		vec3 offset = 0.45f * left;
		offset.y -= 0.5f;
		vec3 incremental_offset = offset / intersection_time;
		base_transformation += offset;
		for (int i = 0; i < NUM_INSTANCES; i++)
		{
			vec3 final_translate = base_transformation;
			vec3 displacement;
			float instance_time = i * pseudo_time;
			displacement.x = velocity.x * instance_time;
			displacement.y = (-0.5f * GRAVITY * pow(instance_time, 2)) + velocity.y * instance_time;
			displacement.z = velocity.z * instance_time;
			final_translate += displacement;
			//final_translate -= incremental_offset * (float)i;
			mat4 mat = glm::translate(mat4(1.f), final_translate);
			for (int k = 0; k < 4; k++)
			{
				vec4 column = glm::column(mat, k);
				for (int l = 0; l < 4; l++)
				{
					translations.push_back(column[l]);
				}
			}
		}
		//density
		/*vector<pair<float, float>> divisions;
		//time, instances distribution
		divisions.push_back(pair<float, float>(0.10f, 0.40f));
		divisions.push_back(pair<float, float>(0.30f, 0.30f));
		divisions.push_back(pair<float, float>(0.60f, 0.30f));

		float time = 0.f;
		for (int i = 0; i < divisions.size(); i++)
		{
			float time_size = intersection_time * divisions.at(i).first;
			float normal_inst_size = NUM_INSTANCES * divisions.at(i).first;
			int block_size = NUM_INSTANCES * divisions.at(i).second;

			float proportion = normal_inst_size / block_size;
			float target_time = time + time_size;

			for (; time < target_time; time += pseudo_time * proportion)
			{
				vec3 final_translate = base_transformation;
				vec3 displacement;
				displacement.x = velocity.x * time;
				displacement.y = (-0.5f * GRAVITY * pow(time, 2)) + velocity.y * time;
				displacement.z = velocity.z * time;
				final_translate += displacement;
				final_translate -= vec3(0.f, incremental_offset.y * time, 0.f);
				mat4 mat = glm::translate(mat4(1.f), final_translate);
				for (int k = 0; k < 4; k++)
				{
					vec4 column = glm::column(mat, k);
					for (int l = 0; l < 4; l++)
					{
						translations.push_back(column[l]);
					}
				}
			}
		}*/


		



		glBindAttribLocation(getProgramID(), 1, "aTransMat");
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_INSTANCES * 4 * 4 * sizeof(GLfloat), &translations[0]);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(0));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 4));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 8));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 12));
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);

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
}