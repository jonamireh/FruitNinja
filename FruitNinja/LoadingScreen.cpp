#include "LoadingScreen.h"
#include "World.h"


LoadingScreen::LoadingScreen(std::string filePath)
{
	shader = TextureDebugShader();
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(assetPath + filePath);
	screen = new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_bonzai84 = TextShader(assetPath + "bonzai.ttf", 64);
}


LoadingScreen::~LoadingScreen()
{
	delete screen;
}

void LoadingScreen::draw()
{
	float static timePassed = 0;
	timePassed += seconds_passed;
	shader.drawTexture(screen->object(), screen_width, screen_height, glm::vec4(1));
	if (timePassed <= 1.5f)
	{
		_bonzai84.draw_text_right("Press Space to Continue", glm::vec2(screen_width - 20, 20), glm::vec3(254.f / 255.f, 215.f / 255.f, 0));
	}
	if (timePassed > 2.f)
	{
		timePassed = 0;
	}
}