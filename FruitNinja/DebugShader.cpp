#include "DebugShader.h"
#include "World.h"

using namespace glm;
using namespace std;

DebugShader::DebugShader(string vertShader, string fragShader) : Shader(vertShader, fragShader)
{
	glGenVertexArrays(1, &LINE_VAO);
	glBindVertexArray(LINE_VAO);
	glBindAttribLocation(getProgramID(), 0, "aPosition");
	glGenBuffers(1, &LINE_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, LINE_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &POINT_VAO);
	glBindVertexArray(POINT_VAO);
	glBindAttribLocation(getProgramID(), 0, "aPosition");
	glGenBuffers(1, &POINT_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, POINT_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void DebugShader::drawLine(vec3 p1, vec3 p2, vec3 color, mat4& view_mat)
{	
	int currentId;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentId);
	assert(currentId == getProgramID());
	glBindVertexArray(LINE_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, LINE_VBO);
	GLfloat g_vertex_buffer_data[] = {
		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
	};
	
	glUniform3fv(getUniformHandle("Ucolor"), 1, value_ptr(color));
	glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 800.f)));
	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		getAttributeHandle("aPosition"),                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the line !
	glDrawArrays(GL_LINES, 0, 2); // 2 indices for the 2 end points of 1 line

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void DebugShader::drawPoint(vec3 p, vec3 color, float radius, mat4& view_mat)
{
	int currentId;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentId);
	assert(currentId == getProgramID());
	glBindVertexArray(POINT_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, POINT_VBO);

	GLfloat g_vertex_buffer_data[] = {
		p.x, p.y, p.z,
	};

	glUniform3fv(getUniformHandle("Ucolor"), 1, value_ptr(color));
	glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 800.f)));
	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		getAttributeHandle("aPosition"),                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the line !
	glPointSize(radius);
	glDrawArrays(GL_POINTS, 0, 1); // 1 indices for the 1 point

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void DebugShader::drawPoints(vector<float> points, vec3 color, mat4& view_mat)
{
	int currentId;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentId);
	assert(currentId == getProgramID());
	glBindVertexArray(POINT_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, POINT_VBO);

	glUniform3fv(getUniformHandle("Ucolor"), 1, value_ptr(color));
	glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 800.f)));
	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));

	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), &points[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		getAttributeHandle("aPosition"),                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the line !
	glPointSize(1.f);
	glDrawArrays(GL_POINTS, 0, points.size() / 3); // 1 indices for the points
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}