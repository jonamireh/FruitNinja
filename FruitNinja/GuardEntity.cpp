#include "GuardEntity.h"
#include "World.h"
#include <glm/geometric.hpp>
#include "GameEntity.h"
#include "AudioManager.h"
#include "main.h"
#include "FrustrumCulling.h"
#include "EntityBox.h"

using namespace glm;
using namespace std;

#define DETECTION_OUTER_RADIUS 15.f
#define DETECTION_INNER_RADIUS 7.f
#define COS_ANGLE 60.f

GuardEntity::GuardEntity(glm::vec3 position, MeshSet* mesh, GuardPuppeteer* puppeteer, std::vector<glm::vec3> control_points,
	float move_speed, bool linear_curve, bool armored) : GameEntity(position, mesh),
	move_component(*this, control_points, move_speed, linear_curve), front(0.f, 0.f, 1.f), animComponent(this, WALKIN), is_armored(armored)

{
	_puppeteer = puppeteer;
	current_animation = mesh->getAnimations()[0];
}

GuardEntity::GuardEntity(glm::vec3 position, MeshSet* mesh, GuardPuppeteer* puppeteer, glm::vec3 dir, bool armored)
	: GameEntity(position, mesh), front(0.f, 0.f, 1.f), move_component(*this, dir), animComponent(this, IDLE), is_armored(armored)

{
	_puppeteer = puppeteer;
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
	if (_is_dying && animComponent.areYouDoneYet()) {
		std::cout << "Yo I'm dead\n";
		list = UNSET_DRAW(list);
	}
	else if (_puppeteer == nullptr || _is_dying) {
		animComponent.update();
	}

    inner_bounding_box.center = bounding_box.center;

	if (walk_channel) AudioManager::instance()->updateChannelPosition(walk_channel, getPosition());
}

bool GuardEntity::check_view(ChewyEntity* chewy, std::vector<GameEntity*> entities)
{
	vec3 lookAt = bounding_box.center + 2.f * move_component.direction - vec3(0.f, .5f, 0.f);
    mat4 view = glm::lookAt(bounding_box.center + move_component.direction, lookAt, vec3(0.f, 1.f, 0.f));

	vector<GameEntity*> just_chewy;
	just_chewy.push_back(chewy);

	vector<GameEntity*> entities_in_view = get_objects_in_view(just_chewy, view, true);

	if (entities_in_view.size() > 0)
	{
		entities_in_view = get_objects_in_view(entities, view, true);
		EntityBox chewy_bb = chewy->bounding_box;
		
		bool hidden = false;
		float chewy_distance = glm::distance(getPosition(), chewy->getPosition());
		for (int i = 0; i < entities_in_view.size(); i++)
		{
			if (entities_in_view.at(i) != chewy)
			{
				EntityBox bb = entities_in_view.at(i)->bounding_box;
				vector<vec3> points = chewy->bounding_box.get_points();
				float results = 0.f;
				for (int i = 0; i < 8; i++)
				{
					pair<bool, float> result = obb_ray(bounding_box.center, normalize(points[i] - bounding_box.center), bb);
					if (result.first)
					{
						if (chewy_distance > result.second)
						{
							results += 0.125;
						}
					}
				}
				if (results >= 0.5)
				{
					hidden = true;
					break;
				}
			}
		}
		//seen
		if (!hidden)
		{
  			animComponent.setCurrentAnimation(IDLE);
			static_movement = true;
			return true;
		}
	}
	return false;
}

GuardEntity::~GuardEntity() {
	stopWalkSound();
}

void GuardEntity::collision(GameEntity* entity)
{
    if (typeid(ChewyEntity) == typeid(*entity) && world->getState() == HIDDEN)
    {
		animComponent.setCurrentAnimation(IDLE);
		static_movement = true;
		world->zoom_on_guard(this);
		bounding_box = inner_bounding_box;
    }
}

void GuardEntity::goAheadAndKillYourself() {
	_is_dying = true;
	static_movement = true;
	stopWalkSound();
	animComponent.setCurrentAnimation(DYING);
	animComponent.update();
}

std::vector<std::vector<glm::mat4>>* GuardEntity::getBoneTransformations() {
	if (_is_dying || _puppeteer == nullptr) {
		return animComponent.basicAnimation.getBoneTransformations();
	}
	else {
		if (static_movement) {
			return _puppeteer->getIdleBoneTransform();
		}
		else {
			return _puppeteer->getWalkerBoneTransform();
		}
	}
}

bool GuardEntity::is_dying() {
	return _is_dying;
}
