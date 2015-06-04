#include "ObstacleEntity.h"

ObstacleEntity::ObstacleEntity()
{
}


ObstacleEntity::ObstacleEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : GameEntity(position, mesh)
{
    //collision_response = true;
}

void ObstacleEntity::update()
{
}

void ObstacleEntity::collision(std::shared_ptr<GameEntity> entity)
{
    //if (!collision_response)
    //    return;
    //// if no collision then stop.
    //if (!entity->bounding_box.box_collision(bounding_box))
    //    return;

    //glm::vec3 pos = getPosition();
    //setPosition(glm::vec3(pos.x, last_position.y, pos.z));

    //if (!entity->bounding_box.box_collision(this->bounding_box))
    //{
    //    setPosition(glm::vec3(pos.x, entity->bounding_box.half_height + entity->bounding_box.center.y + bounding_box.half_height, pos.z));
    //    collision_response = false;
    //}
}