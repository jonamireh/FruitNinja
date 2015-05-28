#pragma once
#include "Shader.h"
#include "TextureDebugShader.h"

class HUD
{
	TextureDebugShader shader;
	std::shared_ptr<tdogl::Texture> arrow_hud;
	std::shared_ptr<tdogl::Texture> power_bar_frame;
	std::shared_ptr<tdogl::Texture> power_bar_bar;
	std::shared_ptr<ChewyEntity> chewy;
public:
	HUD();
	HUD(std::shared_ptr<ChewyEntity> chewy);
	~HUD();
	void draw();
};

