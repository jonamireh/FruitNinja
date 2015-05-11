#include "GuardEntity.h"

GuardEntity::GuardEntity() : move_component(*this, 0.f, glm::vec3(0.f, 0.f, 0.f))
{

}


GuardEntity::GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, float time, glm::vec3 direction) : GameEntity(position, mesh), move_component(*this, time, direction)
{

}

void GuardEntity::update()
{
	move_component.update();
}