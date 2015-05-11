//#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

class GBuffer
{
public:
	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};
	//GBuffer();

	//~GBuffer();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void BindForReading();
	void BindAllForReading();
	void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

	void StartFrame();
	void BindForGeomPass();
	void BindForStencilPass();
	void BindForLightPass();
	void BindForFinalPass();

private:

	GLuint m_fbo;
	GLuint m_textures[GBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;
	GLuint m_finalTexture;
};