#include "LightEntity.h"

void LightEntity::update()
{
}

void LightEntity::disableFlameSound()
{
	flame_channel->stop();
}