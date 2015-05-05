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

Mesh::Mesh(std::vector<glm::vec3> *vd, std::vector<glm::vec3> *nd, std::vector<unsigned int> *id, aiMaterial* material, std::vector<TextureData> *td, std::vector<glm::vec2> *tc)
{
	verts = *vd;
	normals = *nd;
	indices = *id;
	if (td)
		textures = *td;
	if (tc)
		texCoords = *tc;

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

	//verts position
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(vec3),
		&verts[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	//normals
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(vec3),
		&normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	//if texture coords
	if (tc && tc->size() > 0) {
		glGenBuffers(1, &VBO3);
		glBindBuffer(GL_ARRAY_BUFFER, VBO3);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size()*sizeof(vec2),
			&texCoords[0], GL_STATIC_DRAW);
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	}

	glGenBuffers(1, &IND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
	 indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glDisableVertexAttribArray(0); // Disable our Vertex Array Object  
	glBindVertexArray(0); // Disable our Vertex Buffer Object 
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &IND);
}

shared_ptr<BoundingBox> Mesh::getBoundingBox()
{
    if (cache == nullptr)
    {
        float min_x, max_x,
            min_y, max_y,
            min_z, max_z;
        min_x = max_x = verts.at(0).x;
        min_y = max_y = verts.at(0).y;
        min_z = max_z = verts.at(0).z;
        for (int i = 0; i < verts.size(); i++)
        {
            if (verts.at(i).x < min_x) min_x = verts.at(i).x;
            if (verts.at(i).x > max_x) max_x = verts.at(i).x;
            if (verts.at(i).y < min_y) min_y = verts.at(i).y;
            if (verts.at(i).y > max_y) max_y = verts.at(i).y;
            if (verts.at(i).z < min_z) min_z = verts.at(i).z;
            if (verts.at(i).z > max_z) max_z = verts.at(i).z;
        }
        cache = shared_ptr<BoundingBox>(new BoundingBox(glm::vec3(min_x, min_y, min_z), glm::vec3(max_x, max_y, max_z)));
    }
    return cache;
}
