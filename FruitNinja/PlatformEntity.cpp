#include "PlatformEntity.h"
#include <glm/gtx/string_cast.hpp>


PlatformEntity::PlatformEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points, float move_speed) : 
GameEntity(position, mesh), movement_component(*this, control_points, move_speed, true)
{

}

void PlatformEntity::update()
{
    movement_component.update(false);
}

void PlatformEntity::collision(GameEntity* entity)
{
    if (typeid(ChewyEntity) == typeid(*entity))
    {
		glm::vec3 pos = entity->getPosition();

		if (pos.y - entity->bounding_box.half_height < bounding_box.get_lower_bound().y) {
			pos.y = bounding_box.get_upper_bound().y + entity->bounding_box.half_height;
			entity->velocity.y = movement_component.move_speed * movement_component.direction.y * 1.2;
		}

		pos += seconds_passed * movement_component.move_speed * movement_component.direction;

		entity->setPosition(pos);
    }
}