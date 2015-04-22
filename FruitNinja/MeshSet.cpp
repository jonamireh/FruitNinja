
#include "MeshSet.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
//#include <SDL_image.h>
#include <iostream> 
#include <assert.h>

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
	std::vector<VertexData> data;
	std::vector<unsigned int> indices;
	std::vector<TextureData> textures;
	aiColor4D col;
	aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &col);
	glm::vec3 defaultColor(col.r, col.g, col.b);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		VertexData tmp;
		glm::vec3 tmpVec;
		//position
		tmpVec.x = mesh->mVertices[i].x;
		tmpVec.y = mesh->mVertices[i].y;
		tmpVec.z = mesh->mVertices[i].z;
		tmp.pos = tmpVec;
		//normals
		tmpVec.x = mesh->mNormals[i].x;
		tmpVec.y = mesh->mNormals[i].y;
		tmpVec.z = mesh->mNormals[i].z;
		tmp.normal = tmpVec;
		//color
		if (mesh->mColors[0]) {
			tmpVec.x = mesh->mColors[0][i].r;
			tmpVec.y = mesh->mColors[0][i].g;
			tmpVec.z = mesh->mColors[0][i].b;
		}
		else {
			tmpVec = defaultColor;//.x = tmpVec.y = tmpVec.z = 0.7;
		}
		tmp.color = tmpVec;
		//texture coordinates
		if (mesh->mTextureCoords[0]) {
			tmpVec.x = mesh->mTextureCoords[0][i].x;
			tmpVec.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			tmpVec.x = tmpVec.y = tmpVec.z = 0.0;
		}
		tmp.texCoord = glm::vec2(tmpVec.x, tmpVec.y);
		data.push_back(tmp);
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
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
		TextureData tmp;
		tmp.id = loadTexture(str.C_Str());
		tmp.type = 0;
		textures.push_back(tmp);
	}
	meshes.push_back(new Mesh(&data, &indices, mat, &textures));
}

//This function was copied from off the internet, shouldn't be turned in
unsigned int MeshSet::loadTexture(const char* filename) {
	/*unsigned int num;
	glGenTextures(1, &num);
	SDL_Surface* img = IMG_Load(filename);
	if (img == NULL) {
		std::cout << "img was not loaded" << std::endl;
		return -1;
	}
	SDL_PixelFormat form = { NULL, NULL, 32, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff, 0, 255 };
	SDL_Surface* img2 = SDL_ConvertSurface(img, &form, SDL_SWSURFACE);
	if (img2 == NULL) {
		std::cout << "img2 was not loaded" << std::endl;
		return -1;
	}
	glBindTexture(GL_TEXTURE_2D, num);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img2->w, img2->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, img2->pixels);
	SDL_FreeSurface(img);
	SDL_FreeSurface(img2);
	return num;*/
	return 0;
}

MeshSet::MeshSet(std::string filename) {
	const aiScene* scene = aiImportFile(filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene) {
		std::cout << aiGetErrorString();
		return;
	}
	recursiveProcess(scene->mRootNode, scene);
}

MeshSet::~MeshSet() {
	for (int i = 0; i < meshes.size(); i++)
		delete meshes[i];
}

std::vector<Mesh*>& MeshSet::getMeshes() {
	return meshes;
}

