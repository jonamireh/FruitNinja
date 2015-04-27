#include "DebugShader.h"
#include "World.h"

using namespace glm;
using namespace std;

DebugShader::DebugShader(string vertShader, string fragShader) : Shader(vertShader, fragShader) {}

void DebugShader::drawLine(vec3 p1, vec3 p2, vec3 color, mat4& view_mat)
{	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	static const GLfloat g_vertex_buffer_data[] = {
		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
	};
	
	glUniform3fv(getUniformHandle("Ucolor"), 1, value_ptr(color));
	glUniformMatrix4fv(getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(perspective((float)radians(45.0), screen_width / screen_height, 0.1f, 800.f)));
	glUniformMatrix4fv(getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(view_mat));

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}