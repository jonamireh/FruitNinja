#pragma once
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Material.h"

struct VertexData {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoord;
};
struct TextureData {
	GLuint id;
	GLuint type;
};

class Mesh {
public:
	Mesh(std::vector<VertexData> *vd, std::vector<unsigned int> *id, aiMaterial* material, std::vector<TextureData> *td = NULL);
	~Mesh();
	GLuint VAO;
	GLuint VBO;
	GLuint IND;

	std::vector<VertexData> data;
	std::vector<TextureData> textures;
	std::vector<unsigned int> indices;

	Material mat;
private:

	bool checkError(std::string msg);
};
