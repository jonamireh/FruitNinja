#include "GuardEntity.h"

using namespace glm;
using namespace std;

#define DETECTION_OUTER_RADIUS 15.f
#define DETECTION_INNER_RADIUS 7.f
#define COS_ANGLE 60.f

GuardEntity::GuardEntity() : move_component(*this, vector<vec3>(), 0.f)
{

}


GuardEntity::GuardEntity(glm::vec3 position, std::shared_ptr<MeshSet> mesh, std::vector<glm::vec3> control_points, float move_speed) : GameEntity(position, mesh, true), move_component(*this, control_points, move_speed), front(0.f, 0.f, 1.f)
{

}

void GuardEntity::update()
{
	transformed_BB.reset();
	getTransformedOuterBoundingBox();
	move_component.update();
}

void GuardEntity::collision(shared_ptr<GameEntity> entity)
{
	shared_ptr<ChewyEntity> chewy = dynamic_pointer_cast<ChewyEntity>(entity);
	if (chewy != nullptr)
	{
		if (glm::distance(entity->getCenter(), this->getCenter()) < DETECTION_INNER_RADIUS)
		{
			chewy->set_material(Material(vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), 10.f));
		}
		else if (glm::distance(entity->getCenter(), this->getCenter()) < DETECTION_OUTER_RADIUS
			&& degrees(acos(dot(normalize(entity->getCenter() - this->getCenter()), normalize(move_component.direction)))) < COS_ANGLE)
		{
			chewy->set_material(Material(vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), 10.f));
		}
	}
}

float GuardEntity::getRadius()
{
	return DETECTION_OUTER_RADIUS;
}

void GuardEntity::check_view(shared_ptr<ChewyEntity> chewy, std::vector<std::shared_ptr<GameEntity>> entities)
{
	vec3 lookAt = position + move_component.direction;
	mat4 view = glm::lookAt(position, lookAt, vec3(0.f, 1.f, 0.f));

	vector<shared_ptr<GameEntity>> just_chewy;
	just_chewy.push_back(chewy);

	vector<shared_ptr<GameEntity>> entities_in_view = get_objects_in_view(just_chewy, view, true);

	if (entities_in_view.size() > 0)
	{
		cout << "CHEWY SEEN" << endl;
		entities_in_view = get_objects_in_view(entities, view, true);
	}
}