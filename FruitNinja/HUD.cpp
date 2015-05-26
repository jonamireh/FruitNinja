#include "HUD.h"
#include "World.h"
#include "TextureDebugShader.h"

HUD::HUD()
{
	shader = TextureDebugShader();
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "arrowHUD.png");
	bmp.flipVertically();
	arrow_hud = std::shared_ptr<tdogl::Texture>(new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE));
	bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "powerBarFrame.png");
	bmp.flipVertically();
	power_bar_frame = std::shared_ptr<tdogl::Texture>(new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE));
	bmp = tdogl::Bitmap::bitmapFromFile(assetPath + "powerBarBar.png");
	bmp.flipVertically();
	power_bar_bar = std::shared_ptr<tdogl::Texture>(new tdogl::Texture(bmp, GL_LINEAR, GL_CLAMP_TO_EDGE));
}


HUD::~HUD()
{
}

void HUD::draw()
{
	shader.drawTexture(arrow_hud->object(), screen_width - arrow_hud->originalWidth() * .5 - 20, screen_height - arrow_hud->originalHeight() * .5 - 20, arrow_hud->originalWidth() * .5, arrow_hud->originalHeight() * .5);
	shader.drawTexture(power_bar_bar->object(), screen_width - power_bar_frame->originalWidth() - 20, screen_height - 40 - arrow_hud->originalHeight() * .5 - power_bar_frame->originalHeight(), power_bar_frame->originalWidth(), power_bar_frame->originalHeight());
	shader.drawTexture(power_bar_frame->object(), screen_width - power_bar_frame->originalWidth() - 20, screen_height - 40 - arrow_hud->originalHeight() * .5 - power_bar_frame->originalHeight(), power_bar_frame->originalWidth(), power_bar_frame->originalHeight());
}