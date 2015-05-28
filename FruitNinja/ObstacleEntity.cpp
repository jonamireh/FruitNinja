#include "ObstacleEntity.h"
#include "PlayerCamera.h"
#include "BoundingBox.h"
#include <memory>

ObstacleEntity::ObstacleEntity() {}


ObstacleEntity::ObstacleEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : GameEntity(position, mesh) {}

void ObstacleEntity::update()
{
	if (!initBox)
	{
		GameEntity::update();
		initBox = true;
	}
}

glm::vec3 ObstacleEntity::getCenter()
{
	if (!centerSet)
	{
		std::shared_ptr<BoundingBox> outer_bb = getTransformedOuterBoundingBox();
		center = (outer_bb->upper_bound + outer_bb->lower_bound) / 2.f;
		centerSet = true;
	}
	return center;
}