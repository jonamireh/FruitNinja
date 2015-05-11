#pragma once

#include <assimp/scene.h>
#include <vector>
#include "Mesh.h"
#include <string>
#include <map>

struct BoneInfo
{
	aiMatrix4x4 transformation;
	std::map<aiAnimation*, aiNodeAnim*> *bone_anim;
	aiNode *bone_pos;
	aiMatrix4x4 bone_offset;
	BoneInfo(aiNode *pos) : bone_offset(), bone_pos(pos), bone_anim(new std::map<aiAnimation*, aiNodeAnim*>), transformation()
	{
	}
};

class MeshSet
{
public:
	MeshSet(std::string filename);
	void processBones(aiNode* node);
	void processAnimations();
	~MeshSet();
	std::vector<Mesh*>& getMeshes();
	std::vector<aiAnimation>& getAnimations();
	std::map<std::string, BoneInfo*> boneInfo;
	aiNode *bone_tree;
	aiMatrix4x4 inverseMat;
private:
	void recursiveProcess(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	unsigned int loadTexture(const char* filename, int width, int height);
	std::vector<Mesh*> meshes;
	std::vector<aiAnimation> animations;
};
