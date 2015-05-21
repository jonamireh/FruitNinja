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
	
}



ProjectileEntity::ProjectileEntity(std::shared_ptr<MeshSet> mesh,
	std::shared_ptr<Camera> camera) : GameEntity(glm::vec3(0, 0, 0), mesh, true), movement(*this, camera), shot(false), timeLeft(ARROW_LIFE_SPAN)
{

}

void ProjectileEntity::update()
{
	transformed_BB.reset();
	getTransformedOuterBoundingBox();
	movement.update();
}

glm::mat4 ProjectileEntity::getModelMat()
{
	glm::mat4 model_trans = translate(glm::mat4(1.0f), position);
	
	glm::mat4 model_scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));

	return model_trans * rot * model_scale;
}
void ProjectileEntity::collision(std::shared_ptr<GameEntity> entity)
{
	if (typeid(*entity) == typeid(LightEntity) || typeid(*entity) == typeid(TestSphere) || typeid(*entity) == typeid(GuardEntity))
	{
		if (entity->getTransformedOuterBoundingBox()->boxes_collide(*getTransformedOuterBoundingBox()))
			entity->list = UNSET_DRAW(entity->list);
	}
}