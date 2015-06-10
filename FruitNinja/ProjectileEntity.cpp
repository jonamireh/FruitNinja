#include "ProjectileEntity.h"
#include "ArcheryCamera.h"
#include "LightEntity.h"
#include "TestSphere.h"
#include "PlatformEntity.h"

#include "GuardEntity.h"
#include "LightEntity.h"
#include "main.h"


ProjectileEntity::ProjectileEntity() : movement(*this, new ArcheryCamera()), shot(false), timeLeft(5.0f)
{
}

ProjectileEntity::~ProjectileEntity()
{
	//not called til out of world scope. sort of a mem leak
	//std::cout << "Arrow destroyed!!!!!!!\n";
}



ProjectileEntity::ProjectileEntity(MeshSet* mesh, Camera* camera)
	: GameEntity(glm::vec3(0, 0, 0), mesh, true), movement(*this, camera), shot(false), timeLeft(ARROW_LIFE_SPAN)
{
	game_speed = SHOOT_TIME_SPEED;
}

void ProjectileEntity::update()
{
	GameEntity::update();
	movement.update();
	glm::vec3 pos = getPosition();
	if (pos.y < 0.0 || pos.x < 0.0 || pos.z < 0.0 || pos.x > 240.0 || pos.z > 240.0) {
		list = UNSET_DRAW(list);
		game_speed = 1.0;
	}
	GameEntity::update();
}

glm::mat4 ProjectileEntity::getModelMat()
{
	glm::mat4 model_trans = translate(glm::mat4(1.0f), bounding_box.center);
	
	float scale = getScale();
	glm::mat4 model_scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));

	return model_trans * rot * model_scale;
}
void ProjectileEntity::collision(GameEntity* entity)
{
	if (typeid(*entity) == typeid(GuardEntity)) {
		GuardEntity* ge = dynamic_cast<GuardEntity*>(entity);
        if (bounding_box.box_collision(entity->inner_bounding_box))
        {
            if (!ge->is_armored)
            {
                ge->goAheadAndKillYourself();
                list = UNSET_DRAW(list);
				AudioManager::instance()->play3D(assetPath + "WW_Arrow_Hit.wav", getPosition(), 10.0f, false);
                return;
            }
			else
			{
				list = UNSET_DRAW(list);
				AudioManager::instance()->play3D(assetPath + "WW_Arrow_Bounce_Metal1.wav", getPosition(), 8.0f, false);
			}
        }
		else
			return;
	}
	else if (typeid(*entity) == typeid(LightEntity)) {
        if (bounding_box.box_collision(entity->inner_bounding_box))
        {
            LightEntity* le = dynamic_cast<LightEntity*>(entity);
            le->light = NULL;
            list = UNSET_DRAW(list);
			AudioManager::instance()->play3D(assetPath + "WW_Arrow_Bounce_Metal1.wav", getPosition(), 8.0f, false);
            return;
        }
        else
            return;
	}

    if (typeid(*entity) == typeid(PlatformEntity))
    {
        list = UNSET_DRAW(list);
		AudioManager::instance()->play3D(assetPath + "WW_Arrow_Bounce_Stone1.wav", getPosition(), 10.0f, false);
        return;
    }

	//get rid of arrow
	if (!(typeid(*entity) == typeid(ChewyEntity))) {
		world->convert_to_collectible(this);
		list = UNSET_DRAW(list);
		game_speed = 1.0;
		AudioManager::instance()->play3D(assetPath + "WW_Arrow_Hit.wav", getPosition(), 10.0f, false);
	}
}