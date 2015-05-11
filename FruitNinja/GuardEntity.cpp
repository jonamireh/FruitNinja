#include "GuardEntity.h"
#include "ChewyEntity.h"
#include "World.h"

using namespace glm;
using namespace std;

#define DETECTION_OUTER_RADIUS 10.f
#define DETECTION_INNER_RADIUS 5.f
#define COS_ANGLE 0.5

GuardEntity::GuardEntity() : move_component(*this, 0.f, glm::vec3(0.f, 0.f, 0.f))
{

}


GuardEntity::GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, float time, glm::vec3 direction) : GameEntity(position, mesh), move_component(*this, time, direction), front(0.f, 0.f, 1.f)
{

}

void GuardEntity::update()
{
	move_component.update();
}

void GuardEntity::collision(shared_ptr<GameEntity> entity)
{
	shared_ptr<ChewyEntity> chewy = dynamic_pointer_cast<ChewyEntity>(entity);
	if (chewy != nullptr)
	{
		cout << degrees(acos(dot(normalize(entity->getCenter() - this->getCenter()), normalize(this->move_component.direction)))) << endl;
		if (glm::distance(entity->getCenter(), this->getCenter()) < DETECTION_INNER_RADIUS)
		{
			chewy->set_material(Material(vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), 10.f));
		}
		else if (glm::distance(entity->getCenter(), this->getCenter()) < DETECTION_OUTER_RADIUS
			&& acos(dot(normalize(entity->getCenter() - this->getCenter()), normalize(this->move_component.direction))) < acos(COS_ANGLE))
		{
			chewy->set_material(Material(vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), 10.f));
		}
	}
}

float GuardEntity::getRadius()
{
	return DETECTION_OUTER_RADIUS;
}