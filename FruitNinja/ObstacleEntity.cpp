#include "ObstacleEntity.h"
#include "PlayerCamera.h"

ObstacleEntity::ObstacleEntity()
{
    GameEntity::update();
}


ObstacleEntity::ObstacleEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : GameEntity(position, mesh)
{

}

void ObstacleEntity::update()
{

}