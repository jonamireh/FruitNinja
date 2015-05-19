#include "FlameParticleShader.h"
#include "World.h"

using namespace glm;
using namespace std;

FlameParticleShader::FlameParticleShader(std::string vertShader, std::string fragShader) : Shader(vertShader, fragShader)
{
	init();
}

void FlameParticleShader::init()
{
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(0.5f);
}

void FlameParticleShader::draw(glm::mat4& view_mat, shared_ptr<Flame> flame)
{
	int currentId;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentId);
	assert(currentId == getProgramID());


	glBindVertexArray(flame->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, flame->pVBO);
	
	glBindAttribLocation(getProgramID(), 0, "aPosition");
	glBufferData(GL_ARRAY_BUFFER, flame->pos.size() * sizeof(float), &flame->pos[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(getAttributeHandle("aPosition"));
	
	glVertexAttribPointer(
		getAttributeHandle("aPosition"),// attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	glBindBuffer(GL_ARRAY_BUFFER, flame->cVBO);

	glBindAttribLocation(getProgramID(), 0, "aColor");
	glBufferData(GL_ARRAY_BUFFER, flame->col.size() * sizeof(float), &flame->col[0], GL_STATIC_DRAW);
	//glEnableVertexAttribArray(getAttributeHandle("aColor"));
	
	glVertexAttribPointer(
		getAttributeHandle("aColor"),// attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 800.f)));
	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));

	// Draw the points !
	glDrawArrays(GL_POINTS, 0, flame->pos.size() / 3);
	//glDisableVertexAttribArray(0);
	glBindVertexArray(0);


}

