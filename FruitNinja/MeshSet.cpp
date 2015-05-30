
#include "MeshSet.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <GL/gl.h>
#include <GL/glew.h>
//#include <SDL_image.h>
#include <iostream> 
#include <assert.h>
#include "World.h"
#include <fstream>

std::map<string, GLuint> MeshSet::savedTextures = std::map<string, GLuint>();

void MeshSet::recursiveProcess(aiNode *node, const aiScene *scene, GLuint texInterpolation, GLuint texWrap) {
	//process
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene, texInterpolation, texWrap);
	}

	//recursion
	for (int i = 0; i < node->mNumChildren; i++) {
		recursiveProcess(node->mChildren[i], scene, texInterpolation, texWrap);
	}
}

void MeshSet::processMesh(aiMesh *mesh, const aiScene *scene, GLuint texInterpolation, GLuint texWrap) {
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<TextureData> textures;
	std::vector<glm::vec2> texCoords;
	std::vector<aiBone> bones;
	std::vector<glm::ivec4> boneIds;
	std::vector<glm::vec4> boneWeights;
	aiColor4D col;
	aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &col);
	glm::vec3 defaultColor(col.r, col.g, col.b);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		glm::vec3 tmpVec;
		//position
		tmpVec.x = mesh->mVertices[i].x;
		tmpVec.y = mesh->mVertices[i].y;
		tmpVec.z = mesh->mVertices[i].z;
		verts.push_back(tmpVec);
		//normals
		tmpVec.x = mesh->mNormals[i].x;
		tmpVec.y = mesh->mNormals[i].y;
		tmpVec.z = mesh->mNormals[i].z;
		normals.push_back(tmpVec);

		//texture coordinates
		
		if (mesh->mTextureCoords[0]) {
			texCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
		}
	}
	//indices
	for (int i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3); // b/c it's triangulated
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	//just using diffuse texture for now...
	for (int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++) {
		aiString str;
		if (mat->GetTexture(aiTextureType_DIFFUSE, i, &str) == AI_SUCCESS) {
			TextureData tmp;
			//---------------------------
			std::string tempStr(assetPath);
			tempStr += str.C_Str();
			if (savedTextures.find(tempStr) == savedTextures.end())
			{
				printf("Using texture %s...\n", str.C_Str());
				tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(tempStr);
				tdogl::Texture* tex = new tdogl::Texture(bmp, texInterpolation, texWrap);
				tmp.id = tex->object();
				texturesToDel.push_back(tex);
				savedTextures[tempStr] = tex->object();
			}
			else
				tmp.id = savedTextures[tempStr];
			//-------------------------------------
			tmp.type = 0;
			textures.push_back(tmp);
		}
		else {
			printf("Texture failed to load: %s\n", str.C_Str());
		}
	}

	if (mesh->HasBones())
	{
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			boneIds.push_back(glm::ivec4(-1));
			boneWeights.push_back(glm::vec4(0));
		}
	}
	// Bones
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		aiBone *bone = mesh->mBones[i];
		bones.push_back(*bone);
		BoneInfo *info = boneInfo.at(bone->mName.C_Str());
		info->bone_offset = bone->mOffsetMatrix;
		for (int j = 0; j < bone->mNumWeights; j++)
		{
			int k = 0;
			while (k < 3 && boneWeights[bone->mWeights[j].mVertexId][k] != 0.0f)
				k++;
			if (k < 4)
			{
				boneIds.data()[bone->mWeights[j].mVertexId][k] = i;
				boneWeights.data()[bone->mWeights[j].mVertexId][k % 4] = bone->mWeights->mWeight;
			}
		}
	}
	for (int i = 0; i < boneWeights.size(); i++)
	{
		glm::normalize(boneWeights[i]);
	}
	// Add mesh to set
	meshes.push_back(new Mesh(&verts, &normals, &indices, mat, &textures, &texCoords, &bones, &boneIds, &boneWeights, &animations));
}


MeshSet::MeshSet(std::string filename, GLuint texInterpolation, GLuint texWrap) {
	const aiScene* scene = aiImportFile(filename.c_str(), aiProcess_GenNormals | aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs /*| aiProcess_LimitBoneWeights*/ | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);
	if (!scene) {
		std::cout << aiGetErrorString();
		return;
	}
	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		animations.push_back(*scene->mAnimations[i]);
	}
	bone_tree = scene->mRootNode;
	inverseMat = scene->mRootNode->mTransformation;
	inverseMat.Inverse();
	processBones(bone_tree);
	processAnimations();
	recursiveProcess(scene->mRootNode, scene, texInterpolation, texWrap);
}

void MeshSet::processBones(aiNode* node)
{
	boneInfo.insert(std::pair<std::string, BoneInfo*>(node->mName.C_Str(), new BoneInfo(node)));
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processBones(node->mChildren[i]);
	}
}

void MeshSet::processAnimations()
{
	for (int i = 0; i < animations.size(); i++)
	{
		for (int j = 0; j < animations[i].mNumChannels; j++)
		{
			BoneInfo *info = boneInfo.at(animations[i].mChannels[j]->mNodeName.C_Str());
			info->bone_anim->insert(std::pair<aiAnimation*, aiNodeAnim*>(&animations[i], animations[i].mChannels[j]));
		}
	}
}

MeshSet::~MeshSet() {
	for (int i = 0; i < meshes.size(); i++)
		delete meshes[i];
	for (int i = 0; i < texturesToDel.size(); i++)
		delete texturesToDel[i];
}

std::vector<Mesh*>& MeshSet::getMeshes() {
	return meshes;
}

std::vector<aiAnimation>& MeshSet::getAnimations()
{
	return animations;
}