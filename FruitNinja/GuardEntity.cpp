#include "GuardEntity.h"

using namespace std;
using namespace glm;


#define DETECTION_OUTER_RADIUS 10.f
#define DETECTION_INNER_RADIUS 5.f
#define COS_ANGLE 0.5

GuardEntity::GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, float time, glm::vec3 direction) : GameEntity(position, mesh), move_component(*this, time, direction)
{

}

void GuardEntity::update()
{
	move_component.update();
}

void GuardEntity::collisionBS(std::shared_ptr<GameEntity> e)
{
	shared_ptr<ChewyEntity> chewy = dynamic_pointer_cast<ChewyEntity>(e);
	if (chewy != nullptr)
	{
		if (glm::distance(e->getCenter(), this->getCenter()) < DETECTION_INNER_RADIUS && dot(normalize(e->getCenter() - this->getCenter()), this->move_component.direction) < COS_ANGLE)
		{
			chewy->set_material(Material(vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), 10.f));
		}
		else if (glm::distance(e->getCenter(), this->getCenter()) < DETECTION_OUTER_RADIUS)
		{
			chewy->set_material(Material(vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), 10.f));
		}
	}
}

float GuardEntity::getRadius()
{
	return DETECTION_OUTER_RADIUS;
=======
>>>>>>> 7a545a1ac5ac84381529fa3f6b73e41bce3cd1b4
}