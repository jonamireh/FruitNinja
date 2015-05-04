
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
	meshes.push_back(new Mesh(&verts, &normals, &indices, mat, &textures, &texCoords));
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

