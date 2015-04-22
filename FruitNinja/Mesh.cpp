#include "Mesh.h"
#include <string>
#include <vector>
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

bool Mesh::checkError(std::string msg) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << msg << ": OpenGL Error: " << error << std::endl;
		return true;
	}
	return false;
}

Mesh::Mesh(std::vector<VertexData> *vd, std::vector<unsigned int> *id, aiMaterial* material, std::vector<TextureData> *td)
{
	data = *vd;
	indices = *id;
	if (td)
		textures = *td;

	aiColor4D color(0.f, 0.f, 0.f, 0.f);
	aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color);
	mat.ambient = vec3(color.r, color.g, color.b);

	aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color);
	mat.diffuse = vec3(color.r, color.g, color.b);

	aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color);
	mat.specular = vec3(color.r, color.g, color.b);

	aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &(mat.shininess));

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(VertexData),
	 &data[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
	 indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IND);
}
