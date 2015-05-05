#pragma once
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Material.h"
#include "BoundingBox.h"

struct TextureData
{
	GLuint id;
	GLuint type;
};

class Mesh
{
public:
	Mesh(std::vector<glm::vec3>* vd, std::vector<glm::vec3>* nd, std::vector<GLuint>* id, aiMaterial* material,
	     std::vector<TextureData>* td = NULL, std::vector<glm::vec2>* tc = NULL, std::vector<aiBone>* bones = NULL, 
		 std::vector<glm::ivec4>* boneIds = NULL, std::vector<glm::vec4>* boneWeights = NULL, std::vector<aiAnimation> *animationData = NULL);
	~Mesh();
	GLuint VAO;
	GLuint VBO;
	GLuint VBO2;
	GLuint VBO3;
	GLuint VBO_BoneID;
	GLuint VBO_BoneWeight;
	GLuint IND;

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<TextureData> textures;
	std::vector<unsigned int> indices;
	std::vector<glm::vec2> texCoords;
	std::vector<aiBone> bones;
	std::vector<glm::ivec4> boneIds;
	std::vector<glm::vec4> boneWeights;
	std::vector<aiAnimation> animations;
	std::vector<glm::mat4> boneTransformations;
	std::shared_ptr<BoundingBox> getBoundingBox();
	Material mat;
private:
	std::shared_ptr<BoundingBox> cache;
	bool checkError(std::string msg);
};
