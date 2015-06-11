#include "TextShader.h"
#include "World.h"
#include <glm/gtc/type_ptr.hpp>

const GLfloat TextShader::g_quad_vertex_buffer_data[] = {
	-1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,	
};

TextShader::TextShader()
{	
}

TextShader::TextShader(std::string fontFile, int size) : Shader("TextVert.glsl", "TextFrag.glsl")
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	FT_Init_FreeType(&freetype);
	FT_New_Face(freetype, fontFile.c_str(), 0, &font);
	FT_Set_Pixel_Sizes(font, 0, size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		//Load character glyph 
		if (FT_Load_Char(font, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		//Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			font->glyph->bitmap.width,
			font->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			font->glyph->bitmap.buffer
			);

		//Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Now store character for later use
		Character character = {
			texture,
			glm::ivec2(font->glyph->bitmap.width, font->glyph->bitmap.rows),
			glm::ivec2(font->glyph->bitmap_left, font->glyph->bitmap_top),
			font->glyph->advance.x
		};
		
		characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//Destroy FreeType once we're finished
	FT_Done_Face(font);
	FT_Done_FreeType(freetype);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Use GL_DYNAMIC_DRAW because we will update vbo's memory quite often
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
}

TextShader::~TextShader()
{
}

void TextShader::draw_text_right(std::string text, glm::vec2 position, const glm::vec3 &color)
{
	int stringLength = 0;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		//Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		stringLength += (ch.advance >> 6);		//Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	position.x -= stringLength;

	//Iterate through all characters
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		GLfloat xpos = position.x + ch.bearing.x;
		GLfloat ypos = position.y - (ch.size.y - ch.bearing.y);

		GLfloat w = ch.size.x;
		GLfloat h = ch.size.y;

		drawTexture(ch.textureID, xpos, ypos, w, h, color);

		//Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		position.x += (ch.advance >> 6);		//Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextShader::draw_text_centered(std::string text, glm::vec2 position, const glm::vec3 &color)
{
	int stringLength = 0;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		//Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		stringLength += (ch.advance >> 6);		//Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	position.x -= stringLength / 2;

	//Iterate through all characters
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		GLfloat xpos = position.x + ch.bearing.x;
		GLfloat ypos = position.y - (ch.size.y - ch.bearing.y);

		GLfloat w = ch.size.x;
		GLfloat h = ch.size.y;

		drawTexture(ch.textureID, xpos, ypos, w, h, color);

		//Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		position.x += (ch.advance >> 6);		//Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextShader::draw_text_left(std::string text, glm::vec2 position, const glm::vec3 &color)
{

	//Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		GLfloat xpos = position.x + ch.bearing.x;
		GLfloat ypos = position.y - (ch.size.y - ch.bearing.y);

		GLfloat w = ch.size.x;
		GLfloat h = ch.size.y;

		drawTexture(ch.textureID, xpos, ypos, w, h, color);

		//Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		position.x += (ch.advance >> 6);		//Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

/*
Draws a specified texture starting at the initial location and expanding the width to the right and height upwards.
*/
void TextShader::drawTexture(GLuint textureId, int xloc, int yloc, int width, int height, float percent, const glm::vec3 &color)
{
	glUseProgram(getProgramID());

	glViewport(xloc, yloc, width, height);

	if (DEBUG_MODE)
		check_gl_error("TextureDebugShader error part");

	glBindVertexArray(VAO);

	if (DEBUG_MODE)
		check_gl_error("TextureDebugShader error part 2");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);

	if (DEBUG_MODE)
		check_gl_error("TextureDebugShader error part 2.5555");

	glBindTexture(GL_TEXTURE_2D, textureId);
	//check_gl_error("TextureDebugShader error part 3");
	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(getUniformHandle("uTexture"), 0);
	glUniform1f(getUniformHandle("percent"), percent);
	glUniform3f(getUniformHandle("uColor"), color.x, color.y, color.z);

	if (DEBUG_MODE)
		check_gl_error("TextureDebugShader error before");

	// Draw the triangles !
	// You have to disable GL_COMPARE_R_TO_TEXTURE above in order to see anything !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
	glViewport(0, 0, screen_width, screen_height);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	
	if (DEBUG_MODE)
		check_gl_error("TextureDebugShader error after");
}

void TextShader::drawTexture(GLuint textureId, int xloc, int yloc, int width, int height, const glm::vec3 &color)
{
	drawTexture(textureId, xloc, yloc, width, height, 1.0f, color);
}

/*
Draws a specified texture at 0, 0 with the specified width and height.
*/
void TextShader::drawTexture(GLuint textureId, int width, int height, const glm::vec3 &color)
{
	drawTexture(textureId, 0, 0, width, height, color);
}

