#include "ObstacleEntity.h"
#include "PlayerCamera.h"
#include "World.h"

ObstacleEntity::ObstacleEntity()
{
    collision_response = true;
    GameEntity::update();
}


ObstacleEntity::ObstacleEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh) : GameEntity(position, mesh)
{

}

void ObstacleEntity::update()
{
    if (collision_response == true)
        setPosition(getPosition() - 1.f * seconds_passed);
}

void ObstacleEntity::collision(std::shared_ptr<GameEntity> entity)
{
    // if no collision then stop.
    if (!entity->bounding_box.box_collision(bounding_box))
        return;

    glm::vec3 pos = getPosition();
    setPosition(glm::vec3(pos.x, last_position.y, pos.z));

    if (!entity->bounding_box.box_collision(this->bounding_box))
    {
        if (entity->bounding_box.center.y > this->bounding_box.center.y)
            this->velocity.y = 0.f;
        else
            collision_response = false;
        return;
    }
}