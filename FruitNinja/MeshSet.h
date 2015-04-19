#pragma once

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include "Mesh.h"
#include <string>

class MeshSet {
public:
	MeshSet(std::string filename);
	~MeshSet();
	std::vector<Mesh*> &getMeshes();
private:
	void recursiveProcess(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene);
	unsigned int loadTexture(const char *filename);
	std::vector<Mesh*> meshes;
};
