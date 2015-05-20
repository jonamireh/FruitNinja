#include "ObstacleEntity.h"
#include "PlayerCamera.h"

ObstacleEntity::ObstacleEntity()
{

}


ObstacleEntity::ObstacleEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : GameEntity(position, mesh)
{

}

void ObstacleEntity::update()
{
	transformed_BB.reset();
	getTransformedOuterBoundingBox();
}