#include "ProjectileEntity.h"
#include "ArcheryCamera.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <glm/gtx/rotate_vector.hpp>

ProjectileEntity::ProjectileEntity() : movement(*this, std::make_shared<ArcheryCamera>())
{

}

ProjectileEntity::~ProjectileEntity()
{
	
}



ProjectileEntity::ProjectileEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh,
	std::shared_ptr<GameEntity> owner, std::shared_ptr<Camera> camera) : GameEntity(position, mesh), owner(owner), movement(*this, camera)
{

}

void ProjectileEntity::update()
{
	movement.update();
}

glm::mat4 ProjectileEntity::getModelMat()
{
	glm::mat4 model_trans = translate(glm::mat4(1.0f), position);
	
	glm::mat4 model_scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));

	return model_trans * rot * model_scale;
}
