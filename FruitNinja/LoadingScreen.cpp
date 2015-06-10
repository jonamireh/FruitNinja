#include "LoadingScreen.h"
#include "World.h"


LoadingScreen::LoadingScreen(std::string filePath, std::string text)
{
	shader = TextureDebugShader();
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(assetPath + filePath);
	screen = new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_bonzai84 = TextShader(assetPath + "bonzai.ttf", 84);
	_bonzai42 = TextShader(assetPath + "bonzai.ttf", 42);
	this->text = text;
}


LoadingScreen::~LoadingScreen()
{
	delete screen;
}

void LoadingScreen::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float static timePassed = -.2;
	timePassed += seconds_passed;
	shader.drawTexture(screen->object(), screen_width, screen_height, glm::vec4(1));
	_bonzai84.draw_text_left(text, glm::vec2(20, screen_height - 70), glm::vec3(254.f / 255.f, 215.f / 255.f, 0));
	if (timePassed <= 1.f)
	{
		_bonzai42.draw_text_right("Press Enter to Continue", glm::vec2(screen_width - 20, 20), glm::vec3(254.f / 255.f, 215.f / 255.f, 0));
	}
	if (timePassed > 1.5f)
	{
		timePassed = 0;
	}
	glDisable(GL_BLEND);
}