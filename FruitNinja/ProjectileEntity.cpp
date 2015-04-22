#include "ProjectileEntity.h"
#include "ArcheryCamera.h"

ProjectileEntity::ProjectileEntity() : movement(*this, std::shared_ptr<Camera>(new ArcheryCamera()))
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