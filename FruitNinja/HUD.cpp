#include "HUD.h"
#include "World.h"
#include "TextureDebugShader.h"

HUD::HUD() {}

HUD::HUD(ChewyEntity* chewy)
{
	this->chewy = chewy;
	shader = TextureDebugShader();
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "arrowHUD.png");
	arrow_hud = new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE);
	bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "powerBarFrame.png");
	power_bar_frame = new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE);
	bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "powerBarBar.png");
	power_bar_bar = new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE);
	bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "heart_full.png");
	health_full = new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE);
	bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "heart_empty.png");
	health_empty = new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_bonzai84 = TextShader(assetPath + "bonzai.ttf", 64);
}


HUD::~HUD()
{
	delete arrow_hud;
	delete power_bar_frame;
	delete power_bar_bar;
	delete health_full;
	delete health_empty;
}

void HUD::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader.drawTexture(arrow_hud->object(), screen_width - arrow_hud->originalWidth() * .5 - 20, screen_height - arrow_hud->originalHeight() * .5 - 20, arrow_hud->originalWidth() * .5, arrow_hud->originalHeight() * .5, glm::vec4(1));
	_bonzai84.draw_text_centered(std::to_string(arrow_count), glm::vec2(screen_width - arrow_hud->originalWidth() * .5 + 5, screen_height - arrow_hud->originalHeight() * .5 - 15), arrow_count > 0 ? glm::vec3(1) : glm::vec3(1, 0, 0));

	int temp;
	for (temp = 0; temp < health; temp++)
	{
		shader.drawTextureCentered(health_full->object(), 20 + health_full->originalWidth() * .3 / 2 + temp * (health_full->originalWidth() * .3 + 10), screen_height - 20 - health_full->originalHeight() * .3 / 2, health_full->originalWidth() * .3, health_full->originalHeight() * .3, 1, glm::vec4(1));
	}
	for (; temp < MAX_HEALTH; temp++)
	{
		shader.drawTextureCentered(health_empty->object(), 20 + health_full->originalWidth() * .3 / 2 + temp * (health_full->originalWidth() * .3 + 10), screen_height - 20 - health_full->originalHeight() * .3 / 2, health_empty->originalWidth() * .5, health_empty->originalHeight() * .5, 1, glm::vec4(1));
	}

	if (chewy->moveComponent.archery_cam->in_use)
	{
		shader.drawTexture(power_bar_frame->object(), screen_width - power_bar_frame->originalWidth() - 20, screen_height - 40 - arrow_hud->originalHeight() * .5 - power_bar_frame->originalHeight(), power_bar_frame->originalWidth(), power_bar_frame->originalHeight(), glm::vec4(1));
		shader.drawTexture(power_bar_bar->object(), screen_width - power_bar_frame->originalWidth() - 20, screen_height - 40 - arrow_hud->originalHeight() * .5 - power_bar_frame->originalHeight(), power_bar_frame->originalWidth(), power_bar_frame->originalHeight(), bow_strength, glm::vec4(1));
	}
	glDisable(GL_BLEND);
}