#include "GuardEntity.h"
#include "PlayerCamera.h"

GuardEntity::GuardEntity()
{

}


GuardEntity::GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : GameEntity(position, mesh)
{

}

void GuardEntity::update()
{

}