#include "GateEntity.h"

GateEntity::GateEntity(glm::vec3 position, MeshSet* mesh) : GameEntity(position, mesh)
{

}
void GateEntity::update()
{
	GameEntity::update();
}

void GateEntity::collision(GameEntity* entity)
{

}