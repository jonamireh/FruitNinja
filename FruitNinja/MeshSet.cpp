
#include "MeshSet.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <GL/gl.h>
#include <GL/glew.h>
//#include <SDL_image.h>
#include <iostream> 
#include <assert.h>
#include "tdogl/Texture.h"
#include "tdogl/Bitmap.h"
#include "World.h"
#include <fstream>

void MeshSet::recursiveProcess(aiNode *node, const aiScene *scene) {
	//process
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}

	//recursion
	for (int i = 0; i < node->mNumChildren; i++) {
		recursiveProcess(node->mChildren[i], scene);
	}
}

void MeshSet::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<TextureData> textures;
	std::vector<glm::vec2> texCoords;
	std::vector<aiBone> bones;
	std::vector<glm::ivec4> boneIds1;
	std::vector<glm::vec4> boneWeights1;
	std::vector<glm::ivec4> boneIds2;
	std::vector<glm::vec4> boneWeights2;
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
			printf("Using texture %s...\n", str.C_Str());
			tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(tempStr);

			tdogl::Texture* tex = new tdogl::Texture(bmp);
			tmp.id = tex->object();
			//delete tex;//possible memory leak
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
			boneIds1.push_back(glm::ivec4());
			boneWeights1.push_back(glm::vec4());
			boneIds2.push_back(glm::ivec4());
			boneWeights2.push_back(glm::vec4());
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
			while (k < 3 && boneWeights1[bone->mWeights[j].mVertexId][k % 4] != 0.0f ||
				   k < 7 && boneWeights2[bone->mWeights[j].mVertexId][k % 4] != 0.0f)
			{
				k++;
			}
			if (k < 4)
			{
				boneIds1.data()[bone->mWeights[j].mVertexId][k] = i;
				boneWeights1.data()[bone->mWeights[j].mVertexId][k % 4] = bone->mWeights->mWeight;
			}
			else if (k < 8)
			{
				boneIds2.data()[bone->mWeights[j].mVertexId][k] = i;
				boneWeights2.data()[bone->mWeights[j].mVertexId][k % 4] = bone->mWeights->mWeight;
			}
		}
	}
	for (int i = 0; i < boneWeights1.size(); i++)
	{
		
		glm::normalize(boneWeights1[i]);
	}
	// Add mesh to set
	meshes.push_back(new Mesh(&verts, &normals, &indices, mat, &textures, &texCoords, &bones, &boneIds1, &boneWeights1, &boneIds1, &boneWeights1, &animations));
}

//This function was copied from off the internet, shouldn't be turned in
GLuint MeshSet::loadTexture(const char* filename, int width, int height) {

	GLuint texture = 0;
	/*unsigned char *data;
	FILE *file;
		// open texture data
	file = fopen(filename, "rb");
	if (file == NULL) return 0;

	data = (unsigned char*)malloc(width * height * 4);

	fread(data, width * height * 4, 1, file);
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	
	free(data);*/
	return texture;
}

MeshSet::MeshSet(std::string filename) {
	const aiScene* scene = aiImportFile(filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);
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
	recursiveProcess(scene->mRootNode, scene);
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
}

std::vector<Mesh*>& MeshSet::getMeshes() {
	return meshes;
}

std::vector<aiAnimation>& MeshSet::getAnimations()
{
	return animations;
}