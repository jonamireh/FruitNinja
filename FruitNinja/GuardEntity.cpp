#include "GuardEntity.h"
#include "PlayerCamera.h"

GuardEntity::GuardEntity()
{

}


GuardEntity::GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, Material material) : GameEntity(position, mesh, material)
{

}

void GuardEntity::update()
{

}