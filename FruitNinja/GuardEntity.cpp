#include "GuardEntity.h"
#include "World.h"

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
	GameEntity::update();
	move_component.update();
	GameEntity::update();
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

bool segment_aabb(vec3 center, vec3 w, vec3 h)
{
	vec3 v = vec3(fabs(w.x), fabs(w.y), fabs(w.z));
	vec3 nW = normalize(w);
	if (fabs(center.x) > v.x + h.x)
		return false;
	if (fabs(center.y) > v.y + h.y)
		return false;
	if (fabs(center.z) > v.z + h.z)
		return false;
	if ((fabs(center.y * nW.z - center.z * nW.y)) > (h.y*v.z + h.z*v.y))
		return false;
	if ((fabs(center.x * nW.z - center.z * nW.x)) > (h.x*v.z + h.z*v.x))
		return false;
	if ((fabs(center.x * nW.y - center.y * nW.x)) > (h.x*v.y + h.y*v.x))
		return false;
	return true;
}

void GuardEntity::check_view(shared_ptr<ChewyEntity> chewy, std::vector<std::shared_ptr<GameEntity>> entities)
{
	vec3 lookAt = getCenter() + move_component.direction;
	mat4 view = glm::lookAt(getCenter(), lookAt, vec3(0.f, 1.f, 0.f));

	vector<shared_ptr<GameEntity>> just_chewy;
	just_chewy.push_back(chewy);

	vector<shared_ptr<GameEntity>> entities_in_view = get_objects_in_view(just_chewy, view, true);

	if (entities_in_view.size() > 0)
	{
		entities_in_view = get_objects_in_view(entities, view, true);
		shared_ptr<BoundingBox> chewy_bb = chewy->getTransformedOuterBoundingBox();
		
		vec3 lower_center = (chewy_bb->lower_bound + getCenter()) / 2.f;
		vec3 lower_w = chewy_bb->lower_bound - lower_center;

		vec3 upper_center = (chewy_bb->upper_bound + getCenter()) / 2.f;
		vec3 upper_w = chewy_bb->upper_bound - upper_center;
		chewy->set_material(Material(vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), 10.f));
		bool hidden = false;
		for (int i = 0; i < entities_in_view.size(); i++)
		{
			if (entities_in_view.at(i) != chewy)
			{
				shared_ptr<BoundingBox> bb = entities_in_view.at(i)->getTransformedOuterBoundingBox();
				vec3 h = bb->upper_bound - ((bb->upper_bound + bb->lower_bound) / 2.f);
				vec3 bb_center = (bb->upper_bound + bb->lower_bound) / 2.f - entities_in_view.at(i)->position;
				bb_center.y -= h.y;
				if (segment_aabb(lower_center - entities_in_view.at(i)->position - h.y, lower_w, h) && segment_aabb(upper_center - entities_in_view.at(i)->position - h.y, upper_w, h))
				{
					hidden = true;
					break;
				}
			}
		}
		if (!hidden)
		{
			chewy->set_material(Material(vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), 10.f));
		}
	}
}