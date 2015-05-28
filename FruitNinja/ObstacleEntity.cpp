#include "ObstacleEntity.h"
#include "PlayerCamera.h"
#include "BoundingBox.h"
#include <memory>

ObstacleEntity::ObstacleEntity()
{
	GameEntity::update();
	std::shared_ptr<BoundingBox> outer_bb = getTransformedOuterBoundingBox();
	center = (outer_bb->upper_bound + outer_bb->lower_bound) / 2.f;
}


ObstacleEntity::ObstacleEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : GameEntity(position, mesh)
{

}

void ObstacleEntity::update()
{
	
}

glm::vec3 ObstacleEntity::getCenter()
{
	return center;
}