#include "ProjectileEntity.h"
#include "ArcheryCamera.h"
#include "LightEntity.h"
#include "TestSphere.h"

#include "GuardEntity.h"


ProjectileEntity::ProjectileEntity() : movement(*this, std::make_shared<ArcheryCamera>()), shot(false), timeLeft(5.0f)
{
}

ProjectileEntity::~ProjectileEntity()
{
	//not called til out of world scope. sort of a mem leak
	std::cout << "Arrow destroyed!!!!!!!\n";
}



ProjectileEntity::ProjectileEntity(std::shared_ptr<MeshSet> mesh,
	std::shared_ptr<Camera> camera) : GameEntity(glm::vec3(0, 0, 0), mesh, true), movement(*this, camera), shot(false), timeLeft(ARROW_LIFE_SPAN)
{
	game_speed = 0.2;
}

void ProjectileEntity::update()
{
	GameEntity::update();
	movement.update();
	if (getPosition().y < 0.0) {
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
void ProjectileEntity::collision(std::shared_ptr<GameEntity> entity)
{
	if (entity->bounding_box.box_collision(bounding_box)) {
		if (typeid(*entity) == typeid(LightEntity) || typeid(*entity) == typeid(TestSphere) || typeid(*entity) == typeid(GuardEntity)) {
			//get rid of hit entity
			entity->list = UNSET_DRAW(entity->list);
		}
		//get rid of arrow
		if (!(typeid(*entity) == typeid(ChewyEntity))) {
			list = UNSET_DRAW(list);
			game_speed = 1.0;
		}
	}
}