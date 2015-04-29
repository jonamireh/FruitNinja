#include "World.h"

class OtherMesh
{
public:
	OtherMesh();
	~OtherMesh();
	bool LoadMesh(const std::string& Filename);
	void Render();
private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void InitMesh(const aiMesh* paiMesh,
		std::vector<glm::vec3>& Positions,
		std::vector<glm::vec3>& Normals,
		std::vector<glm::vec2>& TexCoords,
		std::vector<GLuint>& Indices);

	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	#define INVALID_MATERIAL 0xFFFFFFFF
	#define INDEX_BUFFER 0 
	#define POS_VB 1
	#define NORMAL_VB 2
	#define TEXCOORD_VB 3 

	GLuint m_VAO;
	GLuint m_Buffers[4];

	struct MeshEntry {
		MeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		unsigned int BaseVertex;
		unsigned int BaseIndex;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	std::vector<GLuint> m_Entries;
	std::vector<GLuint> m_Textures;
};