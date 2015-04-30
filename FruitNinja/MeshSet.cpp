
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
			//std::cout << "U: " << tmpVec.x << std::endl;
			//std::cout << "V: " << tmpVec.y << std::endl;
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
		if (mat->GetTexture(aiTextureType_DIFFUSE, i, &str) == AI_SUCCESS) {
			TextureData tmp;
			//---------------------------
			tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile("../Assets/Test/TestBoxDiffuse.png");//str.C_Str());
			
			bmp.flipVertically();
			tdogl::Texture* tex = new tdogl::Texture(bmp);
			tmp.id = tex->object();
			//delete tex;//possible memory leak
			//-------------------------------------
			printf("Using texture %s\n", str.C_Str());
			tmp.type = 0;
			textures.push_back(tmp);
		}
		else {
			printf("Texture failed to load: %s\n", str.C_Str());
		}
	}
	meshes.push_back(new Mesh(&data, &indices, mat, &textures));
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

