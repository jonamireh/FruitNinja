#pragma once
#include <string>
#include "TextureDebugShader.h"
#include "TextShader.h"

class LoadingScreen
{
	TextureDebugShader shader;
	tdogl::Texture* screen;
	TextShader _bonzai84;
public:
	LoadingScreen(std::string filePath);
	~LoadingScreen();
	void draw();
};

