#include "GuardEntity.h"
#include "World.h"
#include <glm/geometric.hpp>
#include "GameEntity.h"
#include "AudioManager.h"

using namespace glm;
using namespace std;

#define DETECTION_OUTER_RADIUS 15.f
#define DETECTION_INNER_RADIUS 7.f
#define COS_ANGLE 60.f

GuardEntity::GuardEntity(glm::vec3 position, MeshSet* mesh, std::vector<glm::vec3> control_points,
	float move_speed, bool linear_curve) : GameEntity(position, mesh, true),
	move_component(*this, control_points, move_speed, linear_curve), front(0.f, 0.f, 1.f), animComponent(this, WALKIN)
{
	current_animation = mesh->getAnimations()[0];
}

GuardEntity::GuardEntity(glm::vec3 position, MeshSet* mesh, glm::vec3 dir)
	: GameEntity(position, mesh, true), front(0.f, 0.f, 1.f), move_component(*this, dir), animComponent(this, IDLE)
{
	current_animation = mesh->getAnimations()[0];
	static_movement = true;
}

void GuardEntity::playWalkSound()
{
	walk_channel = AudioManager::instance()->play3D(assetPath + "footstep.wav", getPosition(), 10.0f, true);
}

void GuardEntity::stopWalkSound()
{
	if (walk_channel) walk_channel->stop();
}

void GuardEntity::update()
{
	move_component.update(static_movement);
	animComponent.update();
	if (is_dying && animComponent.areYouDoneYet()) {
		std::cout << "Yo I'm dead\n";
		list = UNSET_DRAW(list);
	}

	if (walk_channel) AudioManager::instance()->updateChannelPosition(walk_channel, getPosition());
}

static pair<bool, float> obb_ray(vec3 origin, vec3 direction, EntityBox bb)
{
	vec3 center = bb.center;
	vec3 h = vec3(bb.half_width, bb.half_height, bb.half_depth);

	float tMin = std::numeric_limits<float>::min();
	float tMax = std::numeric_limits<float>::max();
	vec3 p = center - origin;
	for (int i = 0; i < 3; i++)
	{
		vec3 ai(0.f);
		if (i == 0)
			ai = vec3(1.f, 0.f, 0.f);
		if (i == 1)
			ai = vec3(0.f, 1.f, 0.f);
		if (i == 2)
			ai = vec3(0.f, 0.f, 1.f);

		float e = dot(ai, p);
		float f = dot(ai, direction);

		if (abs(f) > 0.0001f)
		{
			float t1 = (e + h[i]) / f;
			float t2 = (e - h[i]) / f;

			if (t1 > t2)
			{
				//swap
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			tMin = fmax(tMin, t1);
			tMax = fmin(tMax, t2);
			if (tMin > tMax || tMax < 0) {
				return pair<bool, float>(false, 0);
			}
		}
		else if (-1.0f * e - h[i] > 0 || h[i] - e < 0)
			return pair<bool, float>(false, 0);
	}
	if (tMin > 0)
		return pair<bool, float>(true, tMin);
	else
		return pair<bool, float>(false, tMax);
}

bool GuardEntity::check_view(ChewyEntity* chewy, std::vector<GameEntity*> entities)
{
	vec3 lookAt = bounding_box.center + 2.f * move_component.direction;
    mat4 view = glm::lookAt(bounding_box.center + move_component.direction, lookAt, vec3(0.f, 1.f, 0.f));

	vector<GameEntity*> just_chewy;
	just_chewy.push_back(chewy);

	vector<GameEntity*> entities_in_view = get_objects_in_view(just_chewy, view, true);

	if (entities_in_view.size() > 0)
	{
		entities_in_view = get_objects_in_view(entities, view, true);
		EntityBox chewy_bb = chewy->bounding_box;
		
        vec3 chewy_lower_bound = chewy_bb.get_lower_bound();
        vec3 chewy_upper_bound = chewy_bb.get_upper_bound();

		//chewy->set_material(Material(vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 1.f, 0.f), 10.f));
		bool hidden = false;
		for (int i = 0; i < entities_in_view.size(); i++)
		{
			if (entities_in_view.at(i) != chewy)
			{
				EntityBox bb = entities_in_view.at(i)->bounding_box;

                pair<bool, float> lower_result = obb_ray(bounding_box.center, normalize(chewy_lower_bound - bounding_box.center), bb);
                pair<bool, float> upper_result = obb_ray(bounding_box.center, normalize(chewy_upper_bound - bounding_box.center), bb);
                float chewy_distance = glm::distance(bounding_box.center, (chewy_lower_bound + chewy_upper_bound) / 2.f);
				if (lower_result.first && upper_result.first)
				{
					if (chewy_distance > lower_result.second && chewy_distance > upper_result.second)
					{
						hidden = true;
						break;
					}
				}
			}
		}
		//seen
		if (!hidden)
		{

			animComponent.setCurrentAnimation(IDLE);
			static_movement = true;
			//chewy->set_material(Material(vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), vec3(1.f, 1.f, 0.f), 10.f));
			return true;
		}
	}
	return false;
}

GuardEntity::~GuardEntity() {
	stopWalkSound();
}

void GuardEntity::goAheadAndKillYourself() {
	is_dying = true;
	static_movement = true;
	stopWalkSound();
	animComponent.setCurrentAnimation(DYING);
}