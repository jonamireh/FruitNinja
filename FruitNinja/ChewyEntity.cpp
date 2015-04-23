#include "ChewyEntity.h"
#include "PlayerCamera.h"

ChewyEntity::ChewyEntity() : moveComponent(*this, std::shared_ptr<Camera>(new PlayerCamera()))
{
	
}

ChewyEntity::ChewyEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::shared_ptr<Camera> camera) : GameEntity(position, mesh), moveComponent(*this, camera)
{
	
}

void ChewyEntity::update()
{
	moveComponent.update();
}