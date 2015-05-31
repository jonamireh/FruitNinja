#include "World.h"
#include "DirLightShader.h"		

DirLightShader::DirLightShader(GBuffer* gbuffer) : Shader("lightVert.glsl", "dirLightFrag.glsl"), gbuffer(gbuffer)
{
	vector<GLfloat> vertices = { -1, -1, 0, // bottom left corner
		-1, 1, 0, // top left corner
		1, 1, 0, // top right corner
		1, -1, 0 }; // bottom right corner

	vector<GLuint> indices = { 0, 1, 2, // first triangle (bottom left - top left - top right)
		0, 2, 3 }; // second triangle (bottom left - top right - bottom right)

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);


	glGenBuffers(1, &IND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindAttribLocation(getProgramID(), 0, "aPosition");

	model_handle = getUniformHandle("uModelMatrix");
	view_handle = getUniformHandle("uViewMatrix");
	proj_handle = getUniformHandle("uProjMatrix");
	pos_map_handle = getUniformHandle("posMap");
	color_map_handle = getUniformHandle("colMap");
	normal_map_handle = getUniformHandle("norMap");
	eye_handle = getUniformHandle("uEye");
	size_handle = getUniformHandle("uSize");
}

DirLightShader::~DirLightShader() {}

void DirLightShader::draw(std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<GameEntity>> ents, std::vector<Light*> lights)
{
	
}
void DirLightShader::draw(glm::mat4& view_mat, std::shared_ptr<GameEntity> entity)
{
	
}

void DirLightShader::pass(std::shared_ptr<Camera> camera)
{
	gbuffer->BindForLightPass();

	glUseProgram(getProgramID());

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBindVertexArray(VAO);

	glUniform1i(pos_map_handle, GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glUniform1i(color_map_handle, GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glUniform1i(normal_map_handle, GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);

	glm::vec3 eye = camera->cameraPosition;

	glUniform3f(eye_handle, eye.x, eye.y, eye.z);
	glUniform2f(size_handle, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	glUniformMatrix4fv(model_handle, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(view_handle, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(proj_handle, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
	check_gl_error("rend before");

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	check_gl_error("rend after");



	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}