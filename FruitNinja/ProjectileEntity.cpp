#include "ProjectileEntity.h"
#include "ArcheryCamera.h"
#include "LightEntity.h"
#include "TestSphere.h"

#include "GuardEntity.h"
#include "LightEntity.h"


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
	if (entity->bounding_box.box_collision(bounding_box)) {
		if (typeid(*entity) == typeid(TestSphere)) {
			//get rid of hit entity
			entity->list = UNSET_DRAW(entity->list);
		} 
		else if (typeid(*entity) == typeid(GuardEntity)) {
			GuardEntity* ge = dynamic_cast<GuardEntity*>(entity);
			ge->goAheadAndKillYourself();
		}
		else if (typeid(*entity) == typeid(LightEntity)) {
			LightEntity* le = dynamic_cast<LightEntity*>(entity);
			le->light = NULL;
		}
		//get rid of arrow
		if (!(typeid(*entity) == typeid(ChewyEntity))) {
			list = UNSET_DRAW(list);
			game_speed = 1.0;
			AudioManager::instance()->play3D(assetPath + "arrow_hit.wav", getPosition(), 10.0f, false);
		}
	}
}