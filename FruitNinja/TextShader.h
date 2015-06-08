#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Shader.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "TextureDebugShader.h"

struct Character {
	GLuint textureID;	//ID handle of the glyph texture
	glm::ivec2 size;			//Size of glyph
	glm::ivec2 bearing;		//Offset from baseline to left/top of glyph
	GLuint advance;		//Offset to advance to next glyph
};

class TextShader : public Shader
{	
	FT_Library freetype;
	FT_Face font;
	GLuint VAO;
	GLuint quad_vertexbuffer;
	GLuint vbo;
	std::map<GLchar, Character> characters;
	static const GLfloat g_quad_vertex_buffer_data[];

	void TextShader::drawTexture(GLuint textureId, int xloc, int yloc, int width, int height, float percent, const glm::vec3 &color);
	void TextShader::drawTexture(GLuint textureId, int xloc, int yloc, int width, int height, const glm::vec3 &color);
	void TextShader::drawTexture(GLuint textureId, int width, int height, const glm::vec3 &color);

public:
	TextShader();
	TextShader(std::string fontFile, int size);
	~TextShader();
	void draw_text_right(std::string text, glm::vec2 position, const glm::vec3 &color);
	void draw_text_centered(std::string text, glm::vec2 position, const glm::vec3 &color);
	void draw_text_left(std::string text, glm::vec2 position, const glm::vec3 &color);
};

