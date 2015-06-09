#pragma once
#include <string>
#include "TextureDebugShader.h"
#include "TextShader.h"

class LoadingScreen
{
	TextureDebugShader shader;
	tdogl::Texture* screen;
	TextShader _bonzai42;
	TextShader _bonzai84;
	std::string text;
public:
	LoadingScreen(std::string filePath, std::string text);
	~LoadingScreen();
	void draw();
};

