#pragma once
#include "Shader.h"
#include "TextureDebugShader.h"
#include "TextShader.h"

class HUD
{
	TextureDebugShader shader;
	TextShader _bonzai84;
	tdogl::Texture* arrow_hud;
	tdogl::Texture* power_bar_frame;
	tdogl::Texture* power_bar_bar;
	ChewyEntity* chewy;
public:
	HUD();
	HUD(ChewyEntity* chewy);
	~HUD();
	void draw();
};

